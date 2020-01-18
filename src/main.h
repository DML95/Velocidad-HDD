#ifndef MAIN_H
#define MAIN_H

	#include<cstdio>
	#include<string>
	#include <memory>

    #include "winapi.h"

	#include "temporizador.h"
	#include "controlesEspeciales.h"
	#include "controlTabla.h"
	#include "clase.h"
	#include "menu.h"

	class Main{
		private:
			static const std::string nameClass;

			static std::shared_ptr<Main> main;

			//clase GUI
			std::shared_ptr<Clase> clase;

			//GUI
			std::shared_ptr<Ventana> ventana;
			std::shared_ptr<Tabla> tabla;

			//Nucleo
			std::shared_ptr<ControlTabla> controlTabla;

			//temporizador
			std::shared_ptr<Temporizador> temporizador;

			//menu
			std::shared_ptr<Menu> mainMenu;
			std::shared_ptr<Menu> propertiesMenu;
			std::shared_ptr<Menu> fileMenu;

			//genera un cuadro de dialogo con errores
			static void messageBoxError(std::string error,int num);
			//procesa los eventos individuales
			static WNDPROC event(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
			//constructor
			Main();
			//gestiona la cola de eventos
			int messageLoop();
		public:

			enum idMenu{
				exit,
				random,
				sequential
			};

			//inicia la ejecucion normal del programa
			int run();
			//destructor
			~Main();
			//devuelve una unstancia de la clase
			static std::shared_ptr<Main>& getInstance();
	};

	int main();

#endif
