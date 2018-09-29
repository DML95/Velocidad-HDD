#include "Tabla.h"
#include "Medicion.h"

class Dispositivos{
	private:
		Medicion *aMedicion;
		Tabla *tabla;
		int dispositivos;
		
		void mostrarBytesSegundo(char *valor,float bytes);
		
		void agregarColumnas();
		
		void agregarDispositivo(char dispositivo,int posicion);
		
		void eliminarDispositivo(int dispositivo,int posicion);
		
		char getUnidad(int fila);
		
	public:
		//Constructor
		Dispositivos(Tabla *tabla);
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
};
