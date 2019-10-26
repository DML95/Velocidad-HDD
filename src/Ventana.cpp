#include <windows.h>
#include<iostream>

#include "Ventana.h"

HWND Ventana::crearVentana(LPCTSTR hClase,HWND padre,int estilo){
	HWND ventana=CreateWindowExA(
			0,
			hClase,
			0,
			estilo,
			CW_USEDEFAULT,CW_USEDEFAULT,
			CW_USEDEFAULT,CW_USEDEFAULT,
			padre,
			0,
			0,
			0
		);
	std::clog<<"[Ventana] ventana creada HWND: "<<ventana<<std::endl;
	return ventana;
}

//Constructores
Ventana::Ventana(LPCTSTR hClase,HWND padre){
	std::clog<<"[Ventana] creado ventana padre HWND: "<<padre<<std::endl;
	this->hVentana=crearVentana(hClase,padre,WS_CHILD);
	if(!this->hVentana){
		throw (int)GetLastError();
	}
}

Ventana::Ventana(LPCTSTR hClase){
	std::clog<<"[Ventana] creado ventana"<<std::endl;
	this->hVentana=crearVentana(hClase,0,0);
	if(!this->hVentana){
		throw (int)GetLastError();
	}
}

Ventana::Ventana(HWND hVentana){
	std::clog<<"[Ventana:"<<hVentana<<"] creado ventana"<<std::endl;
	if(IsWindow(hVentana)){
		this->hVentana=hVentana;
	}else{
		throw (int)GetLastError();
	}
}

Ventana::Ventana(){
	std::clog<<"[Ventana:0] creado ventana"<<std::endl;
	this->hVentana=0;
}

//Destructor
Ventana::~Ventana(){
	std::clog<<"[Ventana:"<<this->hVentana<<"] eliminado ventana"<<std::endl;
	DestroyWindow(this->hVentana);
}

//Getter HWND ventana
HWND Ventana::get(){
	return this->hVentana;
}

//Getter y setter estilos
int Ventana::setEstilos(int estilos){
	std::clog<<"[Ventana:"<<this->hVentana<<"] estableciendo ventana"<<
			"\n\testilos: "<<estilos<<std::endl;
	estilos=SetWindowLong(this->hVentana,GWL_STYLE,estilos);
	return estilos;
}

int Ventana::getEstilos(){
	return GetWindowLong(this->hVentana,GWL_STYLE);
}

//Getter y setter estilos extendidos
int Ventana::setEstilosEx(int estilos){
	std::clog<<"[Ventana:"<<this->hVentana<<"] estableciendo ventana"<<
			"\n\testilos extendidos: "<<estilos<<std::endl;
	estilos=SetWindowLong(this->hVentana,GWL_EXSTYLE,estilos);
	return estilos;
}

int Ventana::getEstilosEx(){
	return GetWindowLong(this->hVentana,GWL_EXSTYLE);
}

//Getter y setter tamaño
void Ventana::setTamano(int anchura,int altura){
	std::clog<<"[Ventana:"<<this->hVentana<<"] estableciendo ventana"<<
			"\n\tanchura: "<<anchura<<
			"\n\taltura: "<<altura<<std::endl;
	SetWindowPos(this->hVentana,0,0,0,anchura,altura,SWP_NOMOVE|SWP_NOZORDER);
}

//Getter y setter posicion
void Ventana::setPosicion(int x,int y){
	std::clog<<"[Ventana:"<<this->hVentana<<"] estableciendo ventana"<<
			"\n\tx: "<<x<<
			"\n\ty: "<<y<<std::endl;
	SetWindowPos(this->hVentana,0,x,y,0,0,SWP_NOSIZE|SWP_NOZORDER);
}

//Getter setter texto
void Ventana::setTexto(std::string texto){
	std::clog<<"[Ventana:"<<this->hVentana<<"] estableciendo ventana"<<
			"\n\ttexto: "<<texto<<std::endl;
	SetWindowText(this->hVentana,texto.c_str());
}

//Getter setter Padre
void Ventana::setPadre(Ventana *padre){
	SetParent(this->hVentana,padre->get());
}

void Ventana::setPadre(HWND hVentana){
	SetParent(this->hVentana,hVentana);
}

//getter setter color
void Ventana::setColor(int color){
	std::clog<<"[Ventana:"<<this->hVentana<<"] estableciendo ventana"<<
			"\n\tcolor: "<<color<<std::endl;
	color&=0xffffff;
	SetClassLongPtr(this->hVentana,GCLP_HBRBACKGROUND,(long long)CreateSolidBrush(color));
}

//Repinta la ventana
void Ventana::repintar(){
	std::clog<<"[Ventana:"<<this->hVentana<<"] repintar ventana"<<std::endl;
	SetWindowPos(this->hVentana,0,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER|SWP_FRAMECHANGED|SWP_NOCOPYBITS);
}

//destruye la ventana
void Ventana::destruir(){
	std::clog<<"[Ventana:"<<this->hVentana<<"] eliminado ventana"<<std::endl;
	DestroyWindow(this->hVentana);
}

//agrega un menu
void Ventana::setMenu(std::shared_ptr<Menu> &menu){
	std::clog<<"[Ventana:"<<this->hVentana<<"] estableciendo menu"<<std::endl;
	this->menu=menu;
	SetMenu(this->hVentana,menu->get());
	DrawMenuBar(this->hVentana);
}
