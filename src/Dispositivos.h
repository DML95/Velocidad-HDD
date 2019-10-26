#ifndef DISPOSITIVOS_H
#define DISPOSITIVOS_H

	#include<vector>
	
	#include "Tabla.h"
	#include "Medicion.h"
	
	//controlador de la tabla de windows
	class Dispositivos{
		private:
			std::vector<Medicion> listMedicion;
			std::shared_ptr<Tabla> tabla;
			int dispositivos;
			
			std::string mostrarBytesSegundo(float bytes);
			
			void agregarColumnas();
			
			void agregarDispositivo(char dispositivo,int posicion);
			
			void eliminarDispositivo(int dispositivo,int posicion);
			
			char getUnidad(int fila);
			
		public:
			//Constructor
			Dispositivos(std::shared_ptr<Tabla> &tabla);
			//Destructor
			~Dispositivos();
			//Actualiza la tabla
			void actualizar();
			//Inicia la medicion de un dispositivo
			void iniciar(int fila);
			//finaliza la medicion de un dispositivo
			void finalizar(int fila);
			//Mustra la medicion en la tabla
			void mostrarMedicion();
			//establece el modo de lectura
			void setMode(Medicion::mode mode);
	};

#endif
