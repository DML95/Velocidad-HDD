#ifndef VENTANAABSTRACT_H
#define VENTANAABSTRACT_H

	#include<string>
	#include<memory>

	#include "winapi.h"

	#include "clase.h"
	#include "menu.h"

	//inicia una tipo de ventan de windows (ventana, tabla, ...)
	class VentanaAbstract{
		protected:
			std::shared_ptr<VentanaAbstract> padre;
			HWND hVentana;

			//constructor abtracto
			VentanaAbstract(LPCTSTR hClase,HWND padre,int estilo);

		public:
			//Destructor
			~VentanaAbstract();

			//Getter HWND ventana
			HWND get();

			//Getter y setter estilos
			int setEstilos(int estilos);

			int getEstilos();

			//Getter y setter estilos extendidos
			int setEstilosEx(int estilos);

			int getEstilosEx();

			//Getter y setter tamaño
			void setTamano(int anchura,int altura);

			//Getter y setter posicion
			void setPosicion(int x,int y);

			//destruye la ventana
			void destruir();

			//envia un evento asincrono a la ventana
			void postMessage(UINT msg,WPARAM wParam,LPARAM lParam);
	};

#endif
