#ifndef TEMPORIZADOR_H
#define TEMPORIZADOR_H
	
	#include "ventana.h"
	
	//crea eventos de temporizacion enviados a una ventana de windows
	class Temporizador{
		private:
			std::shared_ptr<Ventana> ventana;
			UINT_PTR hTemporizador;
		public:
			//Constructor
			Temporizador(std::shared_ptr<Ventana> &ventana,int tiempo);
			
			//Destructor
			~Temporizador();
	};

#endif
