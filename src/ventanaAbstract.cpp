#include<iostream>

#include "ventanaAbstract.h"

VentanaAbstract::VentanaAbstract(const LPCTSTR hClase,const HWND padre,const int estilo){
	this->hVentana=CreateWindowExA(
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
	if(!this->hVentana){
		throw (int)GetLastError();
	}
	std::clog<<"[VentanaAbstract] ventana abstracta creada HWND: "<<this->hVentana<<std::endl;
}

//Destructor
VentanaAbstract::~VentanaAbstract(){
	std::clog<<"[VentanaAbstract:"<<this->hVentana<<"] eliminado ventana"<<std::endl;
	DestroyWindow(this->hVentana);
}

//Getter HWND ventana
HWND VentanaAbstract::get(){
	return this->hVentana;
}

//Getter y setter estilos
int VentanaAbstract::setEstilos(int estilos){
	std::clog<<"[VentanaAbstract:"<<this->hVentana<<"] estableciendo ventana"<<
			"\n\testilos: "<<estilos<<std::endl;
	estilos=SetWindowLong(this->hVentana,GWL_STYLE,estilos);
	return estilos;
}

int VentanaAbstract::getEstilos(){
	return GetWindowLong(this->hVentana,GWL_STYLE);
}

//Getter y setter estilos extendidos
int VentanaAbstract::setEstilosEx(int estilos){
	std::clog<<"[VentanaAbstract:"<<this->hVentana<<"] estableciendo ventana"<<
			"\n\testilos extendidos: "<<estilos<<std::endl;
	estilos=SetWindowLong(this->hVentana,GWL_EXSTYLE,estilos);
	return estilos;
}

int VentanaAbstract::getEstilosEx(){
	return GetWindowLong(this->hVentana,GWL_EXSTYLE);
}

//Getter y setter tamaño
void VentanaAbstract::setTamano(const int anchura,const int altura){
	std::clog<<"[VentanaAbstract:"<<this->hVentana<<"] estableciendo ventana"<<
			"\n\tanchura: "<<anchura<<
			"\n\taltura: "<<altura<<std::endl;
	SetWindowPos(this->hVentana,0,0,0,anchura,altura,SWP_NOMOVE|SWP_NOZORDER);
}

//Getter y setter posicion
void VentanaAbstract::setPosicion(const int x,const int y){
	std::clog<<"[VentanaAbstract:"<<this->hVentana<<"] estableciendo ventana"<<
			"\n\tx: "<<x<<
			"\n\ty: "<<y<<std::endl;
	SetWindowPos(this->hVentana,0,x,y,0,0,SWP_NOSIZE|SWP_NOZORDER);
}

//destruye la ventana
void VentanaAbstract::destruir(){
	std::clog<<"[VentanaAbstract:"<<this->hVentana<<"] eliminado ventana"<<std::endl;
	DestroyWindow(this->hVentana);
}

//envia un evento asincrono a la ventana
void VentanaAbstract::postMessage(const UINT msg,const WPARAM wParam,const LPARAM lParam){
	PostMessage(this->hVentana,msg,wParam,lParam);
}
