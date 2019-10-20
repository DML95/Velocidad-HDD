#include <cstdio>
#include <iostream>
#include <iomanip>
#include <sstream>

#include "Dispositivos.h"

#define COLUMNA_UNIDAD 0
#define COLUMNA_BYTES_SEGUNDO 1
#define COLUMNA_SECTORES_SEGUNDO 2
#define COLUMNA_PORCENTAJE_ERRORES 3
#define COLUMNA_ERROR 4

std::string Dispositivos::mostrarBytesSegundo(float bytes){
	char *unidades=" KMGT";
	while(*unidades&&bytes>=1024){
		bytes/=1024;
		unidades++;
	}
	std::stringstream stream;
	stream<<std::fixed<<std::setprecision(2)<<bytes<<" "<<*unidades<<"B/s";
	return stream.str();
}

void Dispositivos::agregarColumnas(){
	this->tabla->agregarColumna("Unidad",60,COLUMNA_UNIDAD);
	this->tabla->agregarColumna("Bytes/segundo",110,COLUMNA_BYTES_SEGUNDO);
	this->tabla->agregarColumna("IOPS",130,COLUMNA_SECTORES_SEGUNDO);
	this->tabla->agregarColumna("Porcentaje de errores",150,COLUMNA_PORCENTAJE_ERRORES);
	this->tabla->agregarColumna("Error",200,COLUMNA_ERROR);
}

void Dispositivos::agregarDispositivo(char dispositivo,int posicion){
	std::clog<<"[Dispositivos] agregar dispositivo\n\tdispositivo: "<<dispositivo<<
			"\n\tposicion: "<<posicion<<std::endl;
	bool salir=true;
	int cont;
	char unidad[]=" :";
	*unidad=dispositivo;
	this->tabla->agregarFilas(1,posicion);
	this->tabla->setValor(unidad,posicion,COLUMNA_UNIDAD);
	for(cont=0;cont<'Z'-'A'&&salir;cont++){
		if(!this->listMedicion[cont].getDispositivo()){
			this->listMedicion[cont].setDispositivo(dispositivo);
			salir=false;
		}
	}
}

void Dispositivos::eliminarDispositivo(int dispositivo,int posicion){
	std::clog<<"[Dispositivos] eliminar dispositivo\n\tdispositivo: "<<dispositivo<<
			"\n\tposicion: "<<posicion<<std::endl;
	int cont;
	this->tabla->eliminarFilas(1,posicion);
	for(cont=0;cont<'Z'-'A';cont++){
		if(this->listMedicion[cont].getDispositivo()==dispositivo){
			this->listMedicion[cont].setDispositivo(0);
		}
	}
}

char Dispositivos::getUnidad(int fila){
	std::string texto=this->tabla->getValor(fila,COLUMNA_UNIDAD);
	return texto[0];
}

//Constructor
Dispositivos::Dispositivos(std::shared_ptr<Tabla> &tabla):
			tabla(tabla),
			listMedicion('Z'-'A'){
	std::clog<<"[Dispositivos] creando dispositivo"<<std::endl;
	agregarColumnas();
	dispositivos=0;
}

//Destructor
Dispositivos::~Dispositivos(){
	std::clog<<"[Dispositivos] destruyendo dispositivo"<<std::endl;
}

//Actualiza la tabla
void Dispositivos::actualizar(){
	std::clog<<"[Dispositivos] actualizar"<<std::endl;
	char cont,car;
	int dispositivosNuevos=GetLogicalDrives();
	int dispositivosTemporal=dispositivosNuevos;
	this->dispositivos^=dispositivosNuevos;
	for(car='A',cont=0;car<='Z'&&this->dispositivos;car++){
		if(this->dispositivos&1){
			if(dispositivosTemporal&1){
				agregarDispositivo(car,cont);
				cont++;
			}else{
				eliminarDispositivo(car,cont);
			}
		}else if(dispositivosTemporal&1)cont++;
		dispositivosTemporal>>=1;
		this->dispositivos>>=1;
	}
	this->dispositivos=dispositivosNuevos;
}

//Inicia la medicion de un dispositivo
void Dispositivos::iniciar(int fila){
	std::clog<<"[Dispositivos] iniciar fila: "<<fila<<std::endl;
	bool salir=true;
	int cont;
	char unidad=this->getUnidad(fila);
	for(cont=0;cont<'Z'-'A'&&salir&&unidad;cont++){
		if(this->listMedicion[cont].getDispositivo()==unidad){
			try{
				this->listMedicion[cont].iniciar();
				this->tabla->setValor("",fila,COLUMNA_ERROR);
			}catch(int e){
       			this->tabla->setChecked(fila,false);
       			std::clog<<"[Dispositivos] TEST: "<<e<<std::endl;
       			std::string mensajeError="Error al iniciar la unidad";
       			switch(e){
					case ERROR_ACCESS_DENIED:
						mensajeError="Acceso denegado";
						break;
       			}
       			this->tabla->setValor(mensajeError,fila,COLUMNA_ERROR);
			}
			salir=false;
		}
	}
}

//finaliza la medicion de un dispositivo
void Dispositivos::finalizar(int fila){
	std::clog<<"[Dispositivos] finalizar fila: "<<fila<<std::endl;
	char unidad=this->getUnidad(fila);
	for(int cont=0;cont<'Z'-'A'&&unidad;cont++){
		if(this->listMedicion[cont].getDispositivo()==unidad){
			this->listMedicion[cont].detener();
		}
	}
}

//Mustra la medicion en la tabla
void Dispositivos::mostrarMedicion(){
	std::clog<<"[Dispositivos] mostrarMedicion"<<std::endl;
	bool salir;
	char unidad;
	int tam=this->tabla->getNumFilas();
	int operaciones,error;
	for(int fila=0;fila<tam;fila++){
		unidad=this->getUnidad(fila);
		salir=true;
		for(int cont=0;cont<'Z'-'A'&&salir&&unidad;cont++){
			if(this->listMedicion[cont].getDispositivo()==unidad){
				operaciones=this->listMedicion[cont].getOperaciones(&error);
				if(operaciones>=0){
					this->tabla->setValor(std::to_string(operaciones),fila,COLUMNA_SECTORES_SEGUNDO);
					this->tabla->setValor(mostrarBytesSegundo(this->listMedicion[cont].getByteSector()*operaciones),fila,COLUMNA_BYTES_SEGUNDO);
					std::stringstream stream;
					stream<<std::setprecision(2)<<((error+operaciones)?(float)error*100./(error+operaciones):0.)<<" %";
					this->tabla->setValor(stream.str(),fila,COLUMNA_PORCENTAJE_ERRORES);
				}
				salir=false;
			}
		}
	}
}

void Dispositivos::setMode(Medicion::mode mode){
	std::clog<<"[Medicion] estableciendo modo: "<<mode<<std::endl;
	for(int cont=0;cont<'Z'-'A';cont++){
		this->listMedicion[cont].setMode(mode);
	}
}
