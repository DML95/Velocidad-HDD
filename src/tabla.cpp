#include <windows.h>
#include <commctrl.h>
#include<iostream>
#include <cstring>

#include "tabla.h"

Tabla::Tabla(std::shared_ptr<VentanaAbstract> &padre):
			VentanaAbstract(WC_LISTVIEW,padre->get(),WS_CHILD){
	this->padre=padre;
	std::clog<<"[Tabla:"<<this->hVentana<<"] tabla creada"<<std::endl;
}

//Columnas
void Tabla::agregarColumna(const std::string texto,const int anchuraPx,const int posicion){
	std::clog<<"[Tabla:"<<this->hVentana<<"] agregar columna"<<
			"\n\ttexto: "<<texto<<
			"\n\tanchura pixeles: "<<anchuraPx<<
			"\n\tposicion: "<<posicion<<std::endl;
	LVCOLUMN datosColumna;
	datosColumna.mask=LVCF_FMT|LVCF_TEXT|LVCF_WIDTH;
	datosColumna.fmt=LVCFMT_LEFT;
	datosColumna.cx=anchuraPx;
	datosColumna.pszText=(char*)texto.c_str();
	SendMessage(this->hVentana,LVM_INSERTCOLUMN,posicion,(LPARAM)&datosColumna);
}

//Filas
void Tabla::agregarFilas(const int numFilas,const int posicion){
	std::clog<<"[Tabla:"<<this->hVentana<<"] agregar filas"<<
			"\n\tnumero filas: "<<numFilas<<
			"\n\tposicion: "<<posicion<<std::endl;
	LVITEM datosFila;
	std::memset(&datosFila,0,sizeof(LVITEM));
	datosFila.iItem=posicion;
	for(int cont=0;cont<numFilas;cont++){
		SendMessage(this->hVentana,LVM_INSERTITEM,0,(LPARAM)&datosFila);
	}
}

void Tabla::eliminarFilas(const int numFilas,const int posicion){
	std::clog<<"[Tabla:"<<this->hVentana<<"] eliminar filas"<<
			"\n\tnumero filas: "<<numFilas<<
			"\n\tposicion: "<<posicion<<std::endl;
	for(int cont=0;cont<numFilas;cont++){
		SendMessage(this->hVentana,LVM_DELETEITEM,posicion,0);
	}
}

int Tabla::getNumFilas(){
	return SendMessage(this->hVentana,LVM_GETITEMCOUNT,0,0);
}

//Celdas
void Tabla::setValor(const std::string texto,const int fila,const int columna){
	std::clog<<"[Tabla:"<<this->hVentana<<"] set valor"<<
			"\n\ttexto: "<<texto<<
			"\n\tfila: "<<fila<<
			"\n\tcolumna: "<<columna<<std::endl;
	LVITEM datosFila;
	datosFila.pszText=(char*)texto.c_str();
	datosFila.iSubItem=columna;
	SendMessage(this->hVentana,LVM_SETITEMTEXT,fila,(LPARAM)&datosFila);
}

std::string Tabla::getValor(const int fila,const int columna){
	std::string texto(0xff,0);
	LVITEM datosFila;
	datosFila.iSubItem=columna;
	datosFila.pszText=(char*)texto.c_str();
	datosFila.cchTextMax=0xff;
	int size=SendMessage(this->hVentana,LVM_GETITEMTEXT,fila,(LPARAM)&datosFila);
	texto.resize(size);
	return texto;
}

//checkbox
void Tabla::setChecked(const int fila,const bool checked){
	std::clog<<"[Tabla:"<<this->hVentana<<"] checked fila"<<
			"\n\tfila: "<<fila<<
			"\n\tchecked: "<<checked<<std::endl;
	LVITEM datosFila;
	datosFila.state=checked?INDEXTOSTATEIMAGEMASK(2):INDEXTOSTATEIMAGEMASK(1);
	datosFila.stateMask=LVIS_STATEIMAGEMASK;
	SendMessage(this->hVentana,LVM_SETITEMSTATE,fila,(LPARAM)&datosFila);
}

//Estilo
void Tabla::setEstilosTabla(const int estilos){
	SendMessage(this->hVentana,LVM_SETEXTENDEDLISTVIEWSTYLE,0,estilos);
}
