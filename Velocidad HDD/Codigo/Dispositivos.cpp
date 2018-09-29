#include <cstdio>
#include <iostream>

#include "Dispositivos.h"

#define COLUMNA_UNIDAD 0
#define COLUMNA_BYTES_SEGUNDO 1
#define COLUMNA_SECTORES_SEGUNDO 2
#define COLUMNA_PORCENTAJE_ERRORES 3
#define COLUMNA_ERROR 4

void Dispositivos::mostrarBytesSegundo(char *valor,float bytes){
	char *unidades=" KMGT";
	while(*unidades&&bytes>=1024){
		bytes/=1024;
		unidades++;
	}
	std::sprintf(valor,"%.2f %cB/s",bytes,*unidades);
}

void Dispositivos::agregarColumnas(){
	this->tabla->agregarColumna("Unidad",50,COLUMNA_UNIDAD);
	this->tabla->agregarColumna("Bytes/segundo",100,COLUMNA_BYTES_SEGUNDO);
	this->tabla->agregarColumna("Sectores/segundo",100,COLUMNA_SECTORES_SEGUNDO);
	this->tabla->agregarColumna("Porcentaje de errores",120,COLUMNA_PORCENTAJE_ERRORES);
	this->tabla->agregarColumna("Error",200,COLUMNA_ERROR);
}

void Dispositivos::agregarDispositivo(char dispositivo,int posicion){
	bool salir=true;
	int cont;
	char unidad[]=" :";
	*unidad=dispositivo;
	this->tabla->agregarFilas(1,posicion);
	this->tabla->setValor(unidad,posicion,COLUMNA_UNIDAD);
	for(cont=0;cont<'Z'-'A'&&salir;cont++){
		if(!this->aMedicion[cont].getDispositivo()){
			this->aMedicion[cont].setDispositivo(dispositivo);
			salir=false;
		}
	}
}

void Dispositivos::eliminarDispositivo(int dispositivo,int posicion){
	int cont;
	this->tabla->eliminarFilas(1,posicion);
	for(cont=0;cont<'Z'-'A';cont++){
		if(this->aMedicion[cont].getDispositivo()==dispositivo){
			this->aMedicion[cont].setDispositivo(0);
		}
	}
}

char Dispositivos::getUnidad(int fila){
	char unidad[0xf]={0};
	this->tabla->getValor(unidad,0xf,fila,COLUMNA_UNIDAD);
	return *unidad;
}

//Constructor
Dispositivos::Dispositivos(Tabla *tabla){
	this->tabla=tabla;
	agregarColumnas();
	this->aMedicion=new Medicion['Z'-'A'];
	dispositivos=0;
}

//Destructor
Dispositivos::~Dispositivos(){
	delete[] this->aMedicion;
}

//Actualiza la tabla
void Dispositivos::actualizar(){
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
	bool salir=true;
	int cont;
	char unidad=getUnidad(fila);
	for(cont=0;cont<'Z'-'A'&&salir&&unidad;cont++){
		if(this->aMedicion[cont].getDispositivo()==unidad){
			try{
				this->aMedicion[cont].iniciar();
				this->tabla->setValor(0,fila,COLUMNA_ERROR);
			}catch(int e){
       			this->tabla->setChecked(fila,false);
       			this->tabla->setValor("Error al iniciar la unidad",fila,COLUMNA_ERROR);
			}
			salir=false;
		}
	}
}

//finaliza la medicion de un dispositivo
void Dispositivos::finalizar(int fila){
	int cont;
	char unidad=getUnidad(fila);
	for(cont=0;cont<'Z'-'A'&&unidad;cont++){
		if(this->aMedicion[cont].getDispositivo()==unidad){
			this->aMedicion[cont].detener();
		}
	}
}

//Mustra la medicion en la tabla
void Dispositivos::mostrarMedicion(){
	char valor[0xff];
	bool salir;
	char unidad;
	int cont,fila,tam=this->tabla->getNumFilas();
	int val,error;
	//std::cout<<tam<<std::endl;
	for(fila=0;fila<tam;fila++){
		unidad=getUnidad(fila);
		salir=true;
		for(cont=0;cont<'Z'-'A'&&salir&&unidad;cont++){
			if(this->aMedicion[cont].getDispositivo()==unidad){
				val=this->aMedicion[cont].getOperaciones(&error);
				if(val>=0){
					std::sprintf(valor,"%i S/s",val);
					this->tabla->setValor(valor,fila,COLUMNA_SECTORES_SEGUNDO);
					mostrarBytesSegundo(valor,this->aMedicion[cont].getByteSector()*val);
					this->tabla->setValor(valor,fila,COLUMNA_BYTES_SEGUNDO);
					std::sprintf(valor,"%.2f %%",(error+val)?(float)error*100./(error+val):0.);
					this->tabla->setValor(valor,fila,COLUMNA_PORCENTAJE_ERRORES);
				}
				salir=false;
			}
		}
	}
}
