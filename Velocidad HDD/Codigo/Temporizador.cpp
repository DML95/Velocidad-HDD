#include <windows.h>

#include "Temporizador.h"

Temporizador::Temporizador(HWND hVentana,int tiempo){
	this->hTemporizador=SetTimer(hVentana,0,tiempo,0);
	if(!this->hTemporizador){
		throw (int)GetLastError();
	}
}

//Destructor
Temporizador::~Temporizador(){
	KillTimer(0,this->hTemporizador);
}
