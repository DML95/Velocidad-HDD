#include <windows.h>
#include <commctrl.h>
#include<iostream>

#include "ControlesEspeciales.h"

ControlesEspeciales::ControlesEspeciales(int tipo){
	std::clog<<"[ControlesEspeciales] iniciando controles especiales tipo: "<<tipo<<std::endl;
	this->icex.dwSize=sizeof(INITCOMMONCONTROLSEX);
	this->icex.dwICC=tipo;
	if(!InitCommonControlsEx(&icex)){
		throw (int)GetLastError();
	}
}

int ControlesEspeciales::getTipo(){
	return this->icex.dwICC;
}
