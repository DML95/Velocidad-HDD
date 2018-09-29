#include <windows.h>
#include <commctrl.h>

#include "ControlesEspeciales.h"

ControlesEspeciales::ControlesEspeciales(int tipo){
	this->icex.dwSize=sizeof(INITCOMMONCONTROLSEX);
	this->icex.dwICC=tipo;
	if(!InitCommonControlsEx(&icex)){
		throw (int)GetLastError();
	}
}

int ControlesEspeciales::getTipo(){
	return this->icex.dwICC;
}
