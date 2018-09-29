#include "Unidad.h"

class Medicion{
	private:
		bool finHilo;
		int operaciones,error;
		char dispositivo;
		Unidad *unidad;
		HANDLE hHilo;
		
		static int hilo(Medicion *medicion);
		
		static void longLongAleatorio(long long *valor,long long maximo);
		
	public:
		//Constructor
		Medicion();
		
		//Destructor
		~Medicion();
		
		//Getter y setter
		void iniciar();
		
		void detener();
		
		char getDispositivo();
		
		int getOperaciones(int *error);
		
		int getByteSector();
		
		void setDispositivo(char dispositivo);
};
