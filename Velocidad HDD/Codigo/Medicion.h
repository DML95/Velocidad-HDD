#include <atomic>
#include <memory>

#include "Unidad.h"

//crea una operacion de medicion de un dispositivo
class Medicion{
	private:
		std::atomic_bool finHilo;
		std::atomic_int operaciones,error;
		char dispositivo;
		std::shared_ptr<Unidad> unidad;
		HANDLE hHilo;
		
		static int hilo(Medicion *medicion);
		
		static long long longLongAleatorio(long long maximo);
		
	public:
		//Constructor
		Medicion();
		
		//Destructor
		~Medicion();
		
		//inicia la medicion
		void iniciar();
		
		//detiene la medicion
		void detener();
		
		//optiene el dispositivo
		char getDispositivo();
		
		//opetiene las operaciones y errores y reinicia el contador
		int getOperaciones(int *error);
		
		//optiene el tamaño del sector de la unidad medida
		int getByteSector();
		
		//establece el dispositivo
		void setDispositivo(char dispositivo);
};
