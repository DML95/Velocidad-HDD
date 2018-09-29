#include "Ventana.h"

class Tabla:public Ventana{
	private:
		
	public:
		
		//Constructor
		Tabla(HWND padre);
		
		//Columnas
		void agregarColumna(char *texto,int anchuraPx,int posicion);
		
		//Filas
		void agregarFilas(int numFilas,int posicion);
		
		void eliminarFilas(int numFilas,int posicion);
		
		int getNumFilas();
		//Celdas
		void setValor(char *texto,int fila,int columna);
		
		int getValor(char *texto,int tamano,int fila,int columna);
		//checkbox
		void setChecked(int fila,bool checked);
		//Estilo
		void setEstilosTabla(int estilos);
};
