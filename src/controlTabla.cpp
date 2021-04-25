#include <cstdio>
#include <iostream>
#include <iomanip>
#include <sstream>

#include "controlTabla.h"

std::string ControlTabla::toString(const float number,const int decimalNumber){
	std::stringstream stream;
	stream<<std::fixed<<std::setprecision(decimalNumber)<<number;
	return stream.str();
}

std::string ControlTabla::adjustmentUnit(const long long bytes){
	double decimalBytes=bytes;
	unsigned long long cont=0;
	const std::string unidades=" KMGTP";
	for(;decimalBytes>=1024.&&cont<unidades.size();cont++){
		decimalBytes/=1024.;
	}
	std::string unitOut=ControlTabla::toString(decimalBytes,2);
	unitOut.push_back(' ');
	unitOut.push_back(unidades[cont]);
	unitOut.push_back('B');
	return unitOut;
}

void ControlTabla::addColumn(){
	this->tabla->agregarColumna("Nombre",250,ControlTabla::columna::nombre);
	this->tabla->agregarColumna("Unidades",100,ControlTabla::columna::unidades);
	this->tabla->agregarColumna("Capacidad",100,ControlTabla::columna::capacidad);
	this->tabla->agregarColumna("Bytes/segundo",110,ControlTabla::columna::bytesSegundo);
	this->tabla->agregarColumna("IOPS",130,ControlTabla::columna::iops);
	this->tabla->agregarColumna("Porcentaje de errores",150,ControlTabla::columna::porcentajeErrores);
	this->tabla->agregarColumna("Error",300,ControlTabla::columna::error);
}

void ControlTabla::addDevices(){
	const int size=this->devices.size();
	int cont;
	//lista de dispositivos logicos
	for(cont=0;cont<'Z'-'A'&&cont<size;cont++){
		ControlTabla::device *device=&this->devices[cont];
		const char id='A'+cont;
		std::string deviceName="\\\\.\\";
		deviceName.push_back(id);
		deviceName.push_back(':');
		device->name=deviceName;
		device->id=id;
		device->namePhysical=false;
		device->deviceInfo.reset();
		device->deviceMeasuring.reset();
	}
	const int pos=cont;
	//lista de dispostivos fisicos
	for(;cont<size;cont++){
		ControlTabla::device *device=&this->devices[cont];
		const char id=cont-pos;
		std::string deviceName="\\\\.\\physicaldrive"+std::to_string(id);
		device->name=deviceName;
		device->id=id;
		device->namePhysical=true;
		device->deviceInfo.reset();
		device->deviceMeasuring.reset();
	}
}

void ControlTabla::updatePartitionsUnit(ControlTabla::device &device,std::shared_ptr<DeviceInfo> &newDeviceInfo,const int fila){
	std::string logicalDevicesString;
	//unidades logicas que son unidades fisicas
	if(!device.namePhysical&&newDeviceInfo->isPhysical()){
		logicalDevicesString.push_back(device.id);
	//unidades fisicas
	}else{
		int contNumberPartitions=0;
		const int sizeDevices=this->devices.size();
		for(int contDevices=0;contDevices<sizeDevices;contDevices++){
			ControlTabla::device *deviceCont=&this->devices[contDevices];
			//filtramos los dispositivos logicos
			if(!deviceCont->namePhysical&&deviceCont->deviceInfo&&!deviceCont->deviceInfo->isPhysical()){
				const VOLUME_DISK_EXTENTS *partitionsInfo=deviceCont->deviceInfo->getPartitionsInfo().data();
				const int sizePartitions=partitionsInfo->NumberOfDiskExtents;
				//comprovamos que el dispostivo logico pertenezca al ficico y lo a�adimos a la fila
				for(int contPartitions=0;contPartitions<sizePartitions;contPartitions++){
					if((DWORD)device.id==partitionsInfo->Extents[contPartitions].DiskNumber){
						if(contNumberPartitions++)logicalDevicesString.push_back(',');
						logicalDevicesString.push_back(deviceCont->id);
					}
				}
			}
		}
	}
	this->tabla->setValor(logicalDevicesString,fila,ControlTabla::columna::unidades);
}

void ControlTabla::modifiyTable(ControlTabla::device &device,std::shared_ptr<DeviceInfo> &newDeviceInfo,int &contFila){
	//contar fila
	int tempContFila=contFila;
	if(device.deviceInfo&&newDeviceInfo){
		//un dispostivo fisico siempre es fisico pero uno logico no siempre
		const bool deviceIsPhysical=device.namePhysical||device.deviceInfo->isPhysical();
		const bool newDeviceIsPhysical=device.namePhysical||newDeviceInfo->isPhysical();
		//buscamos cambios en el dispostivo
		if(deviceIsPhysical!=newDeviceIsPhysical){
			//creamos o destruimos dependiendo de si ha habiodo una conversion de fisco a logico
			if(newDeviceIsPhysical){
				this->tabla->agregarFilas(1,contFila);
				this->tabla->setValor(newDeviceInfo->getProductId(),contFila,ControlTabla::columna::nombre);
				tempContFila++;
			}else{
				device.deviceMeasuring.reset();
				this->tabla->eliminarFilas(1,contFila);
			}
		}else if(deviceIsPhysical){
			tempContFila++;
		}
		this->updatePartitionsUnit(device,newDeviceInfo,contFila);
	//agregar fila
	}else if(!device.deviceInfo&&newDeviceInfo){
		const bool newDeviceIsPhysical=device.namePhysical||newDeviceInfo->isPhysical();
		if(newDeviceIsPhysical){
			this->tabla->agregarFilas(1,contFila);
			this->tabla->setValor(newDeviceInfo->getProductId(),contFila,ControlTabla::columna::nombre);
			this->updatePartitionsUnit(device,newDeviceInfo,contFila);
			tempContFila++;
		}
	//eliminar fila
	}else if(device.deviceInfo&&!newDeviceInfo){
		bool deviceIsPhysical=device.namePhysical||device.deviceInfo->isPhysical();
		if(deviceIsPhysical){
			device.deviceMeasuring.reset();
			this->tabla->eliminarFilas(1,contFila);
		}
	}
	contFila=tempContFila;
}

ControlTabla::device* ControlTabla::getDevice(const int fila){
	ControlTabla::device *device=NULL;
	int contFila=0;
	const int size=this->devices.size();
	for(int cont=0;cont<size&&!device;cont++){
		ControlTabla::device *deviceCont=&this->devices[cont];
		//filtramos los dispositivos fisicos
		if(deviceCont->namePhysical||(deviceCont->deviceInfo&&deviceCont->deviceInfo->isPhysical())){
			if(fila==contFila){
				device=deviceCont;
			}
			contFila++;
		}
	}
	return device;
}

bool ControlTabla::errorAsync(DeviceMeasuring &deviceMeasuring,const int error,void *param){
	ControlTabla *controlTabla=(ControlTabla*)param;
	int contFila=0;
	bool run=true;
	const int size=controlTabla->devices.size();
	for(int cont=0;cont<size&&run;cont++){
		ControlTabla::device *deviceCont=&controlTabla->devices[cont];
		//filtramos los dispositivos fisicos
		if(deviceCont->namePhysical||(deviceCont->deviceInfo&&deviceCont->deviceInfo->isPhysical())){
			if(deviceCont->deviceMeasuring.get()==&deviceMeasuring){
				controlTabla->ventana->postMessage(ControlTabla::WM_FORCEENDMEASURING,contFila,error);
				run=false;
			}
			contFila++;
		}
	}
	return false;
}

//Constructor
ControlTabla::ControlTabla(std::shared_ptr<Tabla> &tabla,std::shared_ptr<Ventana> &ventana):
            devices(100),
			tabla(tabla),
			ventana(ventana),
			mode(DeviceMeasuring::mode::random){
	std::clog<<"[ControlTabla] creando dispositivo"<<std::endl;
	addColumn();
	addDevices();
}

//Destructor
ControlTabla::~ControlTabla(){
	std::clog<<"[ControlTabla] destruyendo dispositivo"<<std::endl;
}

//Actualiza los dispositivos de la tabla
void ControlTabla::updateDevice(){
	std::clog<<"[ControlTabla] actualizarDispositivos"<<std::endl;
	int contFila=0;
	const int size=this->devices.size();
	for(int cont=0;cont<size;cont++){
		std::shared_ptr<DeviceInfo> deviceUse;
		try{
			deviceUse.reset(new DeviceInfo(this->devices[cont].name));
		}catch(int e){}
		//eliminamos o agregamos elmentos a la tabla segun corresponda y los rellenamos
		this->modifiyTable(this->devices[cont],deviceUse,contFila);
		this->devices[cont].deviceInfo=deviceUse;
	}
}

//Inicia o detiene la medicion de un dispositivo
void ControlTabla::setMeasuring(const int fila,const bool run){
	std::clog<<"[ControlTabla] establece la medicion\n\tfila: "<<fila<<
			"\n\trun: "<<run<<std::endl;
	ControlTabla::device *device=this->getDevice(fila);
	if(run){
		std::string error;
		try{
			DeviceMeasuring *deviceMeasuring=new DeviceMeasuring(device->deviceInfo,this->mode,ControlTabla::errorAsync,this);
			device->deviceMeasuring.reset(deviceMeasuring);
			this->tabla->setValor(ControlTabla::adjustmentUnit(deviceMeasuring->getSize()),
					fila,ControlTabla::columna::capacidad);
		}catch(int e){
			switch(e){
				case ERROR_ACCESS_DENIED:
					error="Acceso denegado";
					break;
				default:
					error="El dispositivo no está listo";
			}
			this->tabla->setChecked(fila,false);
		}
		this->tabla->setValor(error,fila,ControlTabla::columna::error);
	}else{
		device->deviceMeasuring.reset();
		this->tabla->setValor("",fila,ControlTabla::columna::capacidad);
	}
}

//Mustra la medicion en la tabla
void ControlTabla::displayMeasuring(){
	std::clog<<"[ControlTabla] mostrar Medicion"<<std::endl;
	int contFila=0;
	const int size=this->devices.size();
	for(int cont=0;cont<size;cont++){
		const ControlTabla::device *deviceCont=&this->devices[cont];
		//filtramos los dispositivos fisicos
		if(deviceCont->namePhysical||(deviceCont->deviceInfo&&deviceCont->deviceInfo->isPhysical())){
			//mostramos la medida si se esta midiendo
			if(deviceCont->deviceMeasuring){
				DeviceMeasuring::measuring measuring;
				deviceCont->deviceMeasuring->getMeasuringAndReset(measuring);
				this->tabla->setValor(std::to_string(measuring.operations),contFila,ControlTabla::columna::iops);
				this->tabla->setValor(ControlTabla::adjustmentUnit(measuring.bytes),contFila,ControlTabla::columna::bytesSegundo);
				float errors=measuring.operations+measuring.errors;
				if(errors>0.){
					errors=measuring.errors*100./errors;
				}
				this->tabla->setValor(ControlTabla::toString(errors,2)+" %",
						contFila,ControlTabla::columna::porcentajeErrores);

			}
			contFila++;
		}
	}
}

void ControlTabla::setMode(DeviceMeasuring::mode mode){
	std::clog<<"[ControlTabla] estableciendo modo: "<<mode<<std::endl;
	this->mode=mode;
	const int size=this->devices.size();
	for(int cont=0;cont<size;cont++){
		ControlTabla::device *deviceCont=&this->devices[cont];
		if(deviceCont->deviceMeasuring){
			deviceCont->deviceMeasuring->setMode(mode);
		}
	}
}

void ControlTabla::forceEndMeasuring(int fila,int error){
	std::clog<<"[ControlTabla] fuarza el fin de una medicion\n\tfila: "<<fila<<
			"\n\terror: "<<error<<std::endl;
	this->tabla->setValor("No se ha podio continuar con la medicion",fila,ControlTabla::columna::error);
	this->tabla->setChecked(fila,false);
}
