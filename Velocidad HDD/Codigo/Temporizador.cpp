#include <windows.h>
#include<iostream>

#include "Temporizador.h"

Temporizador::Temporizador(std::shared_ptr<Ventana> &ventana,int tiempo):
		ventana(ventana){
	std::clog<<"[Temporizador] creando tiempo: "<<tiempo<<std::endl;
	this->hTemporizador=SetTimer(ventana->get(),0,tiempo,0);
	if(!this->hTemporizador){
		throw (int)GetLastError();
	}
}

//Destructor
Temporizador::~Temporizador(){
	std::clog<<"[Temporizador] destruyendo tiempo: "<<std::endl;
	KillTimer(0,this->hTemporizador);
}
