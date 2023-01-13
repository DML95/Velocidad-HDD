#include <windows.h>
#include<iostream>

#include "clase.h"

Clase::Clase(std::string nombre,const WNDPROC proc){
	std::clog<<"[Clase] creada clase '"<<nombre<<"'"<<std::endl;
	HINSTANCE hInstance=GetModuleHandle(NULL);
	ATOM atom;
	WNDCLASSEX dClase;
	HICON hIcon=LoadIcon(hInstance, MAKEINTRESOURCE(0));
	dClase.cbSize=sizeof(WNDCLASSEX);
	dClase.style=0;
	dClase.lpfnWndProc=proc;
	dClase.cbClsExtra=0;
	dClase.cbWndExtra=0;
	dClase.hInstance=hInstance;
	dClase.hIcon=hIcon;
	dClase.hCursor=0;
	dClase.hbrBackground=(HBRUSH)(COLOR_MENU+1);
	dClase.lpszMenuName=0;
	dClase.lpszClassName=nombre.c_str();
	dClase.hIconSm=hIcon;
	atom=RegisterClassEx(&dClase);
	if(!atom){
		throw (int)GetLastError();
	}
	this->handle=MAKEINTATOM(atom);
}

//Destructor
Clase::~Clase(){
	std::clog<<"[Clase] eliminando clase"<<std::endl;
	UnregisterClass(this->handle,0);
}

//LPCTSTR
LPCTSTR Clase::get(){
	return this->handle;
}

//getter setter Procedimento
WNDPROC Clase::getProc(){
	WNDCLASSEX dClase;
	GetClassInfoEx(0,this->handle,&dClase);
	return dClase.lpfnWndProc;
}
