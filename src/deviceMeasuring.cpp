#include <iostream>
#include <cstring>

#include "deviceMeasuring.h"
#include "overlappeds.h"
#include "utils.h"

bool DeviceMeasuring::readSector(std::vector<char> &buffer,OVERLAPPED *overlapped,DeviceMeasuring &deviceMeasuring,InfoRead &infoRead){
	overlapped->Pointer=(PVOID)(infoRead.sector*deviceMeasuring.sizeSector);
	return ReadFile(deviceMeasuring.handle,buffer.data(),deviceMeasuring.sizeSector*infoRead.sizeBlock,NULL,overlapped);
}

void DeviceMeasuring::mainThread(DeviceMeasuring &deviceMeasuring){
	const std::string name=deviceMeasuring.deviceInfo->get();
	std::clog<<"[DeviceMeasuring: '"<<name<<"' ] iniciando hilo"<<std::endl;
	InfoRead infoRead;
	DWORD size;
	std::vector<char> buffer(deviceMeasuring.sizeSector*deviceMeasuring.sequentialSizeBlock);
	Overlappeds overlappeds;
	deviceMeasuring.run.store(true);
	//se crean operaciones asincronas hasta la destruccion de la clase
	for(HANDLE handleTemp=deviceMeasuring.handle.load();handleTemp&&deviceMeasuring.run.load();handleTemp=deviceMeasuring.handle.load()){
        ((getSector)(deviceMeasuring.getSectorPtr.load()))(infoRead,deviceMeasuring);
		OVERLAPPED *overlapped=overlappeds.wait(false);
		if(overlapped){
			//comprueva la operacion si el overlapped ha sido iniciado
			if(overlapped->Pointer){
				if(GetOverlappedResult(deviceMeasuring.handle,overlapped,&size,false)){
					deviceMeasuring.operations.fetch_add(infoRead.sizeBlock);
				}else{
					const int error=GetLastError();
					switch(error){
						//error en sector
						case ERROR_CRC:
							deviceMeasuring.errors.fetch_add(infoRead.sizeBlock);
							break;
						//gestion de error manual
						default:
							deviceMeasuring.run.store(deviceMeasuring.errorAsync(deviceMeasuring,error,deviceMeasuring.paramError));
					}
				}
			}
			//overlapped->Pointer=(PVOID)(infoRead.sector*deviceMeasuring.sizeSector);
			overlapped->Internal=0;
			overlapped->InternalHigh=0;
			if(DeviceMeasuring::readSector(buffer,overlapped,deviceMeasuring,infoRead)){
				const int error=GetLastError();
				switch(error){
					//no es un error
					case ERROR_IO_PENDING:
						break;
					//gestion de error manual
					default:
						deviceMeasuring.run.store(deviceMeasuring.errorAsync(deviceMeasuring,error,deviceMeasuring.paramError));
				}
			}
		}
	}
	overlappeds.wait(true);
	std::clog<<"[DeviceMeasuring: '"<<name<<"' ] finalizando hilo"<<std::endl;
}

void DeviceMeasuring::getRandonSector(InfoRead &infoRead,DeviceMeasuring &deviceMeasuring){
	unsigned long long out=0;
	for(int cont=0;cont<4&&out<deviceMeasuring.numberSectors;cont++){
		out|=std::rand();
		out<<=16;
	}
	infoRead.sector=deviceMeasuring.numberSectors?out%deviceMeasuring.numberSectors:0;
	infoRead.sizeBlock=1;
}

void DeviceMeasuring::getSequentialSector(InfoRead &infoRead,DeviceMeasuring &deviceMeasuring){
	//seleccionamos un sector multiplo de sequentialSizeBlock
	while(infoRead.sector%deviceMeasuring.sequentialSizeBlock!=0){
		infoRead.sector++;
	}
	infoRead.sector+=deviceMeasuring.sequentialSizeBlock;
	if(infoRead.sector>deviceMeasuring.numberSectors){
		infoRead.sector=0l;
	}
	infoRead.sizeBlock=deviceMeasuring.sequentialSizeBlock;
}

void DeviceMeasuring::init(std::string &device){
	this->handle=CreateFile(
			device.c_str(),
			GENERIC_READ,
			FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,
			0,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL|FILE_FLAG_NO_BUFFERING|FILE_FLAG_OVERLAPPED,
			0);
	if(this->handle==INVALID_HANDLE_VALUE){
		throw (int)GetLastError();
	}
}

void DeviceMeasuring::readGeometryInfo(){
	DISK_GEOMETRY disco;
	unsigned long bytesReturned=0;
	if(DeviceIoControl(this->handle,IOCTL_DISK_GET_DRIVE_GEOMETRY,
			0,0,
			&disco,sizeof(DISK_GEOMETRY),&bytesReturned,NULL)){
		this->numberSectors=(long long)disco.Cylinders.QuadPart*(long long)disco.TracksPerCylinder*(long long)disco.SectorsPerTrack;
		this->sizeSector=disco.BytesPerSector;
	}else{
		this->numberSectors=0l;
		this->sizeSector=0;
	}
	std::clog<<"[DeviceMeasuring: '"<<this->deviceInfo->get()<<"' ] leyendo estructura logica del dispostivo\n\tnumero de sectores: "<<this->numberSectors<<
			"\n\ttamaño del sector: "<<this->sizeSector<<std::endl;
}

void DeviceMeasuring::readAccessAlignmentInfo(){
	STORAGE_PROPERTY_QUERY queryDisk;
	std::memset(&queryDisk, 0, sizeof(STORAGE_PROPERTY_QUERY));
	queryDisk.PropertyId=STORAGE_PROPERTY_ID::StorageAccessAlignmentProperty;
	queryDisk.QueryType=STORAGE_QUERY_TYPE::PropertyStandardQuery;
	STORAGE_ACCESS_ALIGNMENT_DESCRIPTOR alignmentDisk;
	unsigned long bytesReturned=0;
	if(DeviceIoControl(this->handle,IOCTL_STORAGE_QUERY_PROPERTY,
			&queryDisk,sizeof(STORAGE_PROPERTY_QUERY),
			&alignmentDisk,sizeof(STORAGE_ACCESS_ALIGNMENT_DESCRIPTOR),&bytesReturned,NULL)){
		if(this->sizeSector==alignmentDisk.BytesPerLogicalSector){
			this->numberSectors/=alignmentDisk.BytesPerPhysicalSector/this->sizeSector;
			this->sizeSector=alignmentDisk.BytesPerPhysicalSector;
		}
	}
	std::clog<<"[DeviceMeasuring: '"<<this->deviceInfo->get()<<"' ] leyendo estructura fisica del dispostivo\n\tnumero de sectores: "<<this->numberSectors<<
				"\n\ttamaño del sector: "<<this->sizeSector<<std::endl;
}

void DeviceMeasuring::getSequentialSizeBlock(){
	//usamos el logaritmo en base 2 para evitar que en los discos grandes el tamño de bloque sea demasiado grande y en los pequeños demasiado pequeno
	this->sequentialSizeBlock=Utils::log2(this->numberSectors);
	//buscamos el tamaño de bloque mas cercano por abajo que sea multiplo exacto de los sectores del dispositivo
	//se hace para enviar pasarnos o quedarnos cortos al final del disco
	while(this->numberSectors%this->sequentialSizeBlock!=0) {
		this->sequentialSizeBlock--;
	}
}

DeviceMeasuring::DeviceMeasuring(std::shared_ptr<DeviceInfo> &deviceInfo,DeviceMeasuring::mode mode,bool (*errorAsync)(DeviceMeasuring&,int,void*),void *paramError):
		deviceInfo(deviceInfo){
	std::string name=this->deviceInfo->get();
	std::clog<<"[DeviceMeasuring: '"<<name<<"' ] creando medicion"<<std::endl;
	this->errorAsync=errorAsync;
	this->paramError=paramError;
	this->run.store(false);
	this->operations.store(0l);
	this->errors.store(0l);
	this->setMode(mode);
	this->init(name);
	this->readGeometryInfo();
	if(!(this->numberSectors&&this->sizeSector)){
	    std::clog<<"[DeviceMeasuring: '"<<name<<"' ] geometria invalida"<<std::endl;
        CloseHandle(this->handle.load());
		throw 0;
	}
	this->readAccessAlignmentInfo();
	this->getSequentialSizeBlock();
	this->thread=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)DeviceMeasuring::mainThread,(LPVOID)this,0,NULL);
}

DeviceMeasuring::~DeviceMeasuring(){
	std::clog<<"[DeviceMeasuring: '"<<this->deviceInfo->get()<<"' ] eliminado medicion"<<std::endl;
	const HANDLE handleTemp=this->handle.exchange(0);
	CancelIo(handleTemp);
	WaitForSingleObject(this->thread,INFINITE);
	CloseHandle(this->thread);
	CloseHandle(handleTemp);
}

void DeviceMeasuring::getMeasuringAndReset(DeviceMeasuring::measuring &measuring){
	const long long operations=this->operations.exchange(0l);
	measuring.errors=this->errors.exchange(0l);
	measuring.operations=operations;
	measuring.bytes=operations*this->sizeSector;
}

void DeviceMeasuring::setMode(const DeviceMeasuring::mode mode){
	std::clog<<"[DeviceMeasuring: '"<<this->deviceInfo->get()<<"' ] modificando modo: "<<mode<<std::endl;
	switch(mode){
		case DeviceMeasuring::mode::random:
			this->getSectorPtr.store((void*)DeviceMeasuring::getRandonSector);
			break;
		case DeviceMeasuring::mode::sequential:
			this->getSectorPtr.store((void*)DeviceMeasuring::getSequentialSector);
			break;
        default:
            std::clog<<"[DeviceMeasuring: '"<<this->deviceInfo->get()<<"' ] error al modificar modo: "<<mode<<std::endl;
	}
}

long long DeviceMeasuring::getSize(){
	return this->numberSectors*this->sizeSector;
}
