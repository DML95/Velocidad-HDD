#include <windows.h>
#include<iostream>

#include "ventana.h"

//Constructores
Ventana::Ventana(std::shared_ptr<Clase> &clase):
		clase(clase),
		VentanaAbstract(clase->get(),0,0){
	std::clog<<"[Ventana:"<<hVentana<<"] ventana creada"<<std::endl;
}

//getter setter color
void Ventana::setColor(int color){
	std::clog<<"[Ventana:"<<this->hVentana<<"] estableciendo ventana"<<
			"\n\tcolor: "<<color<<std::endl;
	color&=0xffffff;
	SetClassLongPtr(this->hVentana,GCLP_HBRBACKGROUND,(long long)CreateSolidBrush(color));
}

//Getter setter Padre
void Ventana::setPadre(std::shared_ptr<VentanaAbstract> &padre){
	this->padre=padre;
	SetParent(this->hVentana,padre->get());
}

//Getter setter texto
void Ventana::setTexto(std::string texto){
	std::clog<<"[Ventana:"<<this->hVentana<<"] estableciendo ventana"<<
			"\n\ttexto: "<<texto<<std::endl;
	SetWindowText(this->hVentana,texto.c_str());
}

//Repinta la ventana
void Ventana::repintar(){
	std::clog<<"[Ventana:"<<this->hVentana<<"] repintar ventana"<<std::endl;
	SetWindowPos(this->hVentana,0,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER|SWP_FRAMECHANGED|SWP_NOCOPYBITS);
}

//agrega un menu
void Ventana::setMenu(std::shared_ptr<Menu> &menu){
	std::clog<<"[Ventana:"<<this->hVentana<<"] estableciendo menu"<<std::endl;
	this->menu=menu;
	SetMenu(this->hVentana,menu->get());
	DrawMenuBar(this->hVentana);
}
