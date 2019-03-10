#include <iostream>
#include <windows.h>

#include "Medicion.h"

int Medicion::hilo(Medicion *medicion){
	long long sectorAleatorio,numSectores;
	int tamanoSector=medicion->unidad->getByteSector();
	char buffer[2*tamanoSector-1];
	char *bufferAlineado=(char*)((long long)(buffer+tamanoSector-1)&~(tamanoSector-1));
	medicion->unidad->getNumSector(&numSectores);
	while(medicion->finHilo){
		longLongAleatorio(&sectorAleatorio,numSectores);
		if(medicion->unidad->getRegistro(bufferAlineado,sectorAleatorio)){
			medicion->operaciones++;
		}else medicion->error++;
	}
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
	this->hHilo=0;
	this->finHilo=false;
	this->dispositivo=0;
	this->unidad=0;
	this->operaciones=-1;
	this->error=-1;
}

//Destructor
Medicion::~Medicion(){
	detener();
}

//Getter y setter
void Medicion::iniciar(){
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
	if(this->hHilo){
		this->finHilo=false;
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
	int operaciones=this->operaciones;
	*error=this->error;
	if(operaciones>=0){
		this->operaciones=0;
		this->error=0;
	}
	return operaciones;
}

int Medicion::getByteSector(){
	return this->unidad->getByteSector();
}

void Medicion::setDispositivo(char dispositivo){
	this->dispositivo=dispositivo;
}
