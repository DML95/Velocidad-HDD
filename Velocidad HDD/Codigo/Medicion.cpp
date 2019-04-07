#include <iostream>
#include <windows.h>

#include "Medicion.h"

int Medicion::hilo(Medicion *medicion){
	std::clog<<"[Medicion] iniciando hilo unidad: "<<medicion->getDispositivo()<<std::endl;
	long long sectorAleatorio,numSectores;
	int tamanoSector=medicion->unidad->getByteSector();
	char buffer[2*tamanoSector-1];
	char *bufferAlineado=(char*)((long long)(buffer+tamanoSector-1)&~(tamanoSector-1));
	medicion->unidad->getNumSector(&numSectores);
	while(medicion->finHilo){
		longLongAleatorio(&sectorAleatorio,numSectores);
		if(medicion->unidad->getRegistro(bufferAlineado,sectorAleatorio)){
			medicion->operaciones.fetch_add(1);
		}else medicion->error.fetch_add(1);
	}
	std::clog<<"[Medicion] finalizando hilo unidad: "<<medicion->getDispositivo()<<std::endl;
	return 0;
}

void Medicion::longLongAleatorio(long long *valor,long long maximo){
	int cont;
	short *subValor=(short*)valor;
	for(cont=0;cont<4;cont++){
		subValor[cont]=(short)std::rand();
	}
	*valor%=maximo;
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
}

//Destructor
Medicion::~Medicion(){
	std::clog<<"[Medicion] destruyendo medicion"<<std::endl;
}

//Getter y setter
void Medicion::iniciar(){
	std::clog<<"[Medicion] iniciando medicion dispositivo: "<<this->dispositivo<<std::endl;
	long long numSector;
	if(!this->hHilo){
		this->finHilo=true;
		this->unidad=new Unidad(this->dispositivo);
		this->unidad->getNumSector(&numSector);
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
		delete this->unidad;
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
