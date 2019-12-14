#ifndef VENTANA_H
#define VENTANA_H
	
	#include<windows.h>
	#include<string>
	#include<memory>
	
	#include "ventanaAbstract.h"
	#include "clase.h"
	#include "menu.h"
	
	//crea una ventana de windows
	class Ventana:public VentanaAbstract{
		private:
			std::shared_ptr<Clase> clase;
			std::shared_ptr<Menu> menu;
			
		public:
			
			//Constructor
			Ventana(std::shared_ptr<Clase> &clase);
			
			//getter setter color
			void setColor(int color);
			
			//Getter setter Padre
			void setPadre(std::shared_ptr<VentanaAbstract> &padre);
			
			//Getter setter texto
			void setTexto(std::string texto);
			
			//Repinta la ventana
			void repintar();
			
			//agrega un menu
			void setMenu(std::shared_ptr<Menu> &menu);
	};

#endif
