#include <iostream>
#include <windows.h>
#include <vector>

#include "Medicion.h"

int Medicion::hilo(Medicion *medicion){
	std::clog<<"[Medicion] iniciando hilo unidad: "<<medicion->getDispositivo()<<std::endl;
	const int tamanoSector=medicion->unidad->getByteSector();
	DWORD tam=0;
	const HANDLE hUnidad=medicion->unidad->get();
	char buffer[2*tamanoSector-1];
	const char *bufferAlineado=(char*)((long long)(buffer+tamanoSector-1)&~(tamanoSector-1));
	const long long numSectores=medicion->unidad->getNumSector();
	std::vector<HANDLE> listEvent(MAXIMUM_WAIT_OBJECTS);
	std::vector<OVERLAPPED> listOverlapped(MAXIMUM_WAIT_OBJECTS);
	long long sector=0;
	//iniciamos los la lista de eventos
	for(int cont=0;cont<MAXIMUM_WAIT_OBJECTS;++cont){
		HANDLE event=CreateEvent(NULL,true,true,NULL);
		listEvent[cont]=event;
		OVERLAPPED *overlapped=&listOverlapped[cont];
		overlapped->hEvent=event;
		overlapped->Pointer=0;
	}
	//iniciamos el bucle principal
	while(medicion->finHilo){
		medicion->getSector(sector,numSectores);
		const int eventId=WaitForMultipleObjects(listEvent.capacity(),listEvent.data(),false,INFINITE)-WAIT_OBJECT_0;
		if(eventId>=0){
			OVERLAPPED *overlapped=&listOverlapped[eventId];
			if(overlapped->Pointer){
				if(GetOverlappedResult(hUnidad,overlapped,&tam,false)){
					medicion->operaciones.fetch_add(1);
				}else medicion->error.fetch_add(1);
			}
			overlapped->Pointer=(PVOID)(sector*tamanoSector);
			overlapped->Internal=0;
			overlapped->InternalHigh=0;
			medicion->unidad->getRegistro(bufferAlineado,overlapped);
		}else medicion->error.fetch_add(1);
	}
	WaitForMultipleObjects(listEvent.capacity(),listEvent.data(),true,INFINITE);
	//cerramos los la lista de eventos
	for(int cont=0;cont<MAXIMUM_WAIT_OBJECTS;++cont){
		CloseHandle(listEvent[cont]);
	}
	std::clog<<"[Medicion] finalizando hilo unidad: "<<medicion->getDispositivo()<<std::endl;
	return 0;
}

void Medicion::getRandonSector(long long &selectSector,long long numSectors){
	long long out=0;
	for(int cont=0;cont<4&&out<numSectors;cont++){
		out|=std::rand();
		out<<=16;
	}
	selectSector=out%numSectors;
}

void Medicion::getSequentialSector(long long &selectSector,long long numSectors){
	selectSector++;
	selectSector%=numSectors;
}

//Constructor
Medicion::Medicion(){
	std::clog<<"[Medicion] creando medicion"<<std::endl;
	this->hHilo=0;
	this->finHilo=false;
	this->dispositivo=0;
	this->unidad=0;
	this->operaciones=-1;
	this->error=-1;
	this->getSector.exchange(Medicion::getRandonSector);
}

//Destructor
Medicion::~Medicion(){
	std::clog<<"[Medicion] destruyendo medicion"<<std::endl;
}

//Getter y setter
void Medicion::iniciar(){
	std::clog<<"[Medicion] iniciando medicion dispositivo: "<<this->dispositivo<<std::endl;
	if(!this->hHilo){
		this->finHilo=true;
		this->unidad.reset(new Unidad(this->dispositivo));
		this->hHilo=CreateThread(0,0,(LPTHREAD_START_ROUTINE)hilo,this,0,0);
		this->operaciones=0;
		this->error=0;
	}
}

void Medicion::detener(){
	std::clog<<"[Medicion] detener medicion dispositivo: "<<this->dispositivo<<std::endl;
	if(this->hHilo){
		this->finHilo.exchange(false);
		this->unidad->cancelarOperacion();
		WaitForSingleObject(this->hHilo,INFINITE);
		CloseHandle(this->hHilo);
		this->unidad.reset();
		this->unidad=0;
		this->hHilo=0;
		this->operaciones=-1;
		this->error=-1;
	}
}

char Medicion::getDispositivo(){
	return this->dispositivo;
}

int Medicion::getOperaciones(int *error){
	int operaciones=this->operaciones.exchange(-1);
	if(operaciones>=0){
		*error=this->error.exchange(0);
	}
	return operaciones;
}

int Medicion::getByteSector(){
	return this->unidad->getByteSector();
}

void Medicion::setDispositivo(char dispositivo){
	std::clog<<"[Medicion] estableciendo dispositivo: "<<dispositivo<<std::endl;
	this->dispositivo=dispositivo;
}

void Medicion::setMode(Medicion::mode mode){
	switch(mode){
		case Medicion::mode::random:
			this->getSector.exchange(Medicion::getRandonSector);
			break;
		case Medicion::mode::sequential:
			this->getSector.exchange(Medicion::getSequentialSector);
			break;
	}
}
