#include <windows.h>
#include<iostream>

#include "Temporizador.h"

Temporizador::Temporizador(HWND hVentana,int tiempo){
	std::clog<<"[Temporizador] creando tiempo: "<<tiempo<<std::endl;
	this->hTemporizador=SetTimer(hVentana,0,tiempo,0);
	if(!this->hTemporizador){
		throw (int)GetLastError();
	}
}

//Destructor
Temporizador::~Temporizador(){
	std::clog<<"[Temporizador] destruyendo tiempo: "<<std::endl;
	KillTimer(0,this->hTemporizador);
}
