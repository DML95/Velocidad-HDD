#include<string>

#include "Ventana.h"

//crea una tabla de windows
class Tabla:public Ventana{
	private:
		
	public:
		
		//Constructor
		Tabla(HWND padre);
		
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
