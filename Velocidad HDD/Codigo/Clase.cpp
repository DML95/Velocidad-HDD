#include <windows.h>
#include<iostream>

#include "Clase.h"

Clase::Clase(std::string nombre,const WNDPROC proc){
	std::clog<<"[Clase] creada clase '"<<nombre<<"'"<<std::endl;
	ATOM atomo;
	WNDCLASSEX dClase;
	dClase.cbSize=sizeof(WNDCLASSEX);
	dClase.style=0;
	dClase.lpfnWndProc=proc;
	dClase.cbClsExtra=0;
	dClase.cbWndExtra=0;
	dClase.hInstance=0;
	dClase.hIcon=0;
	dClase.hCursor=0;
	dClase.hbrBackground=(HBRUSH)(COLOR_MENU+1);
	dClase.lpszMenuName=0;
	dClase.lpszClassName=nombre.c_str();
	dClase.hIconSm=0;
	atomo=RegisterClassEx(&dClase);
	if(!atomo){
		throw (int)GetLastError();
	}
	this->hClase=MAKEINTATOM(atomo);
}

//Destructor
Clase::~Clase(){
	std::clog<<"[Clase] eliminando clase"<<std::endl;
	UnregisterClass(this->hClase,0);
}

//LPCTSTR
LPCTSTR Clase::getClase(){
	return this->hClase;
}

//getter setter Procedimento
WNDPROC Clase::getProc(){
	WNDCLASSEX dClase;
	GetClassInfoEx(0,this->hClase,&dClase);
	return dClase.lpfnWndProc;
}

void Clase::setProc(WNDPROC proc){
	std::clog<<"[Clase] setProc: "<<proc<<std::endl;
	WNDCLASSEX dClase;
	GetClassInfoEx(0,this->hClase,&dClase);
	dClase.lpfnWndProc=proc;
}

HWND Clase::getHVentana(){
	return FindWindow(this->hClase,NULL);
}
