#ifndef TABLA_H
#define TABLA_H
	
	#include<memory>
	#include<string>
	
	#include "ventanaAbstract.h"
	
	//crea una tabla de windows
	class Tabla:public VentanaAbstract{
		public:
			
			//Constructor
			Tabla(std::shared_ptr<VentanaAbstract> &padre);
			
			//Columnas
			void agregarColumna(std::string texto,int anchuraPx,int posicion);
			
			//Filas
			void agregarFilas(int numFilas,int posicion);
			
			void eliminarFilas(int numFilas,int posicion);
			
			int getNumFilas();
			//Celdas
			void setValor(std::string texto,int fila,int columna);
			
			std::string getValor(int fila,int columna);
			//checkbox
			void setChecked(int fila,bool checked);
			//Estilo
			void setEstilosTabla(int estilos);
	};

#endif
