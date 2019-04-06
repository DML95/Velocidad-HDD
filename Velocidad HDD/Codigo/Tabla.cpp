#include <windows.h>
#include <commctrl.h>
#include<iostream>

#include "Tabla.h"

Tabla::Tabla(HWND padre):Ventana(WC_LISTVIEW,padre){
	std::clog<<"[Tabla:"<<this->hVentana<<"] tabla creada"<<std::endl;
}

//Columnas
void Tabla::agregarColumna(std::string texto,int anchuraPx,int posicion){
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
void Tabla::agregarFilas(int numFilas,int posicion){
	std::clog<<"[Tabla:"<<this->hVentana<<"] agregar filas"<<
			"\n\tnumero filas: "<<numFilas<<
			"\n\tposicion: "<<posicion<<std::endl;
	int cont;
	LVITEM datosFila;
	ZeroMemory(&datosFila,sizeof(LVITEM));
	datosFila.iItem=posicion;
	for(cont=0;cont<numFilas;cont++){
		SendMessage(this->hVentana,LVM_INSERTITEM,0,(LPARAM)&datosFila);
	}
}

void Tabla::eliminarFilas(int numFilas,int posicion){
	std::clog<<"[Tabla:"<<this->hVentana<<"] eliminar filas"<<
			"\n\tnumero filas: "<<numFilas<<
			"\n\tposicion: "<<posicion<<std::endl;
	int cont;
	for(cont=0;cont<numFilas;cont++){
		SendMessage(this->hVentana,LVM_DELETEITEM,posicion,0);
	}
}

int Tabla::getNumFilas(){
	return SendMessage(this->hVentana,LVM_GETITEMCOUNT,0,0);
}

//Celdas
void Tabla::setValor(std::string texto,int fila,int columna){
	std::clog<<"[Tabla:"<<this->hVentana<<"] set valor"<<
			"\n\ttexto: "<<texto<<
			"\n\tfila: "<<fila<<
			"\n\tcolumna: "<<columna<<std::endl;
	LVITEM datosFila;
	datosFila.pszText=(char*)texto.c_str();
	datosFila.iSubItem=columna;
	SendMessage(this->hVentana,LVM_SETITEMTEXT,fila,(LPARAM)&datosFila);
}

std::string Tabla::getValor(int fila,int columna){
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
void Tabla::setChecked(int fila,bool checked){
	std::clog<<"[Tabla:"<<this->hVentana<<"] checked fila"<<
			"\n\tfila: "<<fila<<
			"\n\tchecked: "<<checked<<std::endl;
	LVITEM datosFila;
	datosFila.state=checked?INDEXTOSTATEIMAGEMASK(2):INDEXTOSTATEIMAGEMASK(1);
	datosFila.stateMask=LVIS_STATEIMAGEMASK;
	SendMessage(this->hVentana,LVM_SETITEMSTATE,fila,(LPARAM)&datosFila);
}

//Estilo
void Tabla::setEstilosTabla(int estilos){
	SendMessage(this->hVentana,LVM_SETEXTENDEDLISTVIEWSTYLE,0,estilos);
}
