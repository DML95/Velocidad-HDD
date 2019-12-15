#include <iostream>

#include "deviceMeasuring.h"
#include "overlappeds.h"

bool DeviceMeasuring::readSector(std::vector<char> &buffer,OVERLAPPED *overlapped,DeviceMeasuring *deviceMeasuring){
	return ReadFile(deviceMeasuring->handle,buffer.data(),deviceMeasuring->sizeSector,NULL,overlapped);
}

void DeviceMeasuring::mainThread(DeviceMeasuring *deviceMeasuring){
	const std::string name=deviceMeasuring->deviceInfo->get();
	std::clog<<"[DeviceMeasuring: '"<<name<<"' ] iniciando hilo"<<std::endl;
	long long sector=0;
	DWORD size;
	std::vector<char> buffer(deviceMeasuring->sizeSector);
	Overlappeds overlappeds;
	deviceMeasuring->run.exchange(true);
	//se crean operaciones asincronas hasta la destruccion de la clase
	for(HANDLE handleTemp=deviceMeasuring->handle;handleTemp&&deviceMeasuring->run;handleTemp=deviceMeasuring->handle){
		deviceMeasuring->getSector(sector,deviceMeasuring->numberSectors);
		OVERLAPPED *overlapped=overlappeds.wait(false);
		if(overlapped){
			//comprueva la operacion si el overlapped ha sido iniciado
			if(overlapped->Pointer){
				if(GetOverlappedResult(deviceMeasuring->handle,overlapped,&size,false)){
					deviceMeasuring->operations.fetch_add(1);
				}else{
					const int error=GetLastError();
					switch(error){
						//error en sector
						case ERROR_CRC:
							deviceMeasuring->errors.fetch_add(1);
							break;
						//gestion de error manual
						default:
							deviceMeasuring->run.exchange(deviceMeasuring->errorAsync(deviceMeasuring,error,deviceMeasuring->paramError));
					}
				}
			}
			overlapped->Pointer=(PVOID)(sector*deviceMeasuring->sizeSector);
			overlapped->Internal=0;
			overlapped->InternalHigh=0;
			if(DeviceMeasuring::readSector(buffer,overlapped,deviceMeasuring)){
				const int error=GetLastError();
				switch(error){
					//no es un error
					case ERROR_IO_PENDING:
						break;
					//gestion de error manual
					default:
						deviceMeasuring->run.exchange(deviceMeasuring->errorAsync(deviceMeasuring,error,deviceMeasuring->paramError));
				}
			}
		}
	}
	overlappeds.wait(true);
	std::clog<<"[DeviceMeasuring: '"<<name<<"' ] finalizando hilo"<<std::endl;
}

void DeviceMeasuring::getRandonSector(long long &selectSector,const long long numberSectors){
	long long out=0;
	for(int cont=0;cont<4&&out<numberSectors;cont++){
		out|=std::rand();
		out<<=16;
	}
	selectSector=out%numberSectors;
}

void DeviceMeasuring::getSequentialSector(long long &selectSector,const long long numberSectors){
	selectSector++;
	if(selectSector>numberSectors){
		selectSector=0l;
	}
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
	std::clog<<"[DeviceMeasuring: '"<<this->deviceInfo->get()<<"' ] leyendo estructura del dispostivo\n\tnumero de sectores: "<<this->numberSectors<<
			"\n\ttama�o del sector: "<<this->sizeSector<<std::endl;
}

DeviceMeasuring::DeviceMeasuring(std::shared_ptr<DeviceInfo> &deviceInfo,DeviceMeasuring::mode mode,bool (*errorAsync)(DeviceMeasuring*,int,void*),void *paramError):
		deviceInfo(deviceInfo){
	std::string name=this->deviceInfo->get();
	std::clog<<"[DeviceMeasuring: '"<<name<<"' ] creando medicion"<<std::endl;
	this->errorAsync=errorAsync;
	this->paramError=paramError;
	this->run=false;
	this->operations=0l;
	this->errors=0l;
	this->setMode(mode);
	this->init(name);
	this->readGeometryInfo();
	this->thread=std::thread(DeviceMeasuring::mainThread,this);
}

DeviceMeasuring::~DeviceMeasuring(){
	std::clog<<"[DeviceMeasuring: '"<<this->deviceInfo->get()<<"' ] eliminado medicion"<<std::endl;
	const HANDLE handleTemp=this->handle.exchange(0);
	CancelIo(handleTemp);
	this->thread.join();
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
			this->getSector.exchange(DeviceMeasuring::getRandonSector);
			break;
		case DeviceMeasuring::mode::sequential:
			this->getSector.exchange(DeviceMeasuring::getSequentialSector);
			break;
	}
}

long long DeviceMeasuring::getSize(){
	return this->numberSectors*this->sizeSector;
}