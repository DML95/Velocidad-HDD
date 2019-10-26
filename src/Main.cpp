#include<cstdio>
#include<iostream>
#include<string>
#include <windows.h>
#include <dbt.h>
#include <commctrl.h>
#include <memory>

#include "Temporizador.h"
#include "ControlesEspeciales.h"
#include "Dispositivos.h"
#include "Clase.h"
#include "Main.h"
#include "Menu.h"

#define ID_EXIT 1
#define ID_RANDOM 2
#define ID_SEQUENTIAL 3

static std::shared_ptr<Ventana> ventana;
static std::shared_ptr<Tabla> tabla;
static std::shared_ptr<Dispositivos> dispositivos;
static std::shared_ptr<Temporizador> temporizador;

//menu
static std::shared_ptr<Menu> mainMenu;
static std::shared_ptr<Menu> propertiesMenu;

static void error(std::string error,int num){
	std::string texto="Error clase '"+error+"'\n\nError numero "+std::to_string(num);
	//std::sprintf(texto,"Error clase '%s'\n\nError numero %i",error,num);
	std::clog<<"[Error] "<<texto<<std::endl;
	MessageBox(0,texto.c_str(),"Error",0);
}

//Arranca el bucle de mensajes
static int bucleMensajes(HWND hVentana){
	MSG mensaje;
	while(GetMessage(&mensaje,hVentana,0,0)>0){
		if(!IsDialogMessage(hVentana,&mensaje)){
		    TranslateMessage(&mensaje);
		    DispatchMessage(&mensaje);
		}
	}
	return mensaje.lParam;
}

//Funcion de recepcion de Eventos
static WNDPROC evento(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
	WNDPROC salida=(WNDPROC)DefWindowProc(hWnd, msg, wParam, lParam);;
	switch(msg){
		//Redimension de la ventana
		case WM_SIZE:
			std::clog<<"[evento] WM_SIZE"<<std::endl;
			tabla->setTamano(LOWORD(lParam),HIWORD(lParam));
			break;
		//Evento temporizador
		case WM_TIMER:
			std::srand(std::rand());
			dispositivos->mostrarMedicion();
			break;
		//Notificaciones de la tabla
		case WM_NOTIFY:{
			NMHDR *header = (NMHDR*)lParam;
	        NMLISTVIEW *nmlist = (NMLISTVIEW*)lParam;
	        switch(header->code){
	        	case LVN_ITEMCHANGED:
	        		std::clog<<"[evento] WM_NOTIFY\n\tLVN_ITEMCHANGED"<<std::endl;
	        		switch(nmlist->uNewState&LVIS_STATEIMAGEMASK){
	        			case INDEXTOSTATEIMAGEMASK(BST_CHECKED+1):
	        				dispositivos->iniciar(nmlist->iItem);
	        				
	        				break;
	        			case INDEXTOSTATEIMAGEMASK(BST_UNCHECKED+1):
	        				dispositivos->finalizar(nmlist->iItem);
					}
			}
	        break;
		}
		case WM_COMMAND:{
			std::clog<<"[evento] WM_COMMAND"<<std::endl;
			switch(LOWORD(wParam)){
				case ID_EXIT:
					ventana->destruir();
					break;
				case ID_RANDOM:
					propertiesMenu->setChack(ID_RANDOM,true);
					propertiesMenu->setChack(ID_SEQUENTIAL,false);
					dispositivos->setMode(Medicion::mode::random);
					break;
				case ID_SEQUENTIAL:
					propertiesMenu->setChack(ID_RANDOM,false);
					propertiesMenu->setChack(ID_SEQUENTIAL,true);
					dispositivos->setMode(Medicion::mode::sequential);
					break;
			}
            break;
        }
		//Cambio de dispositivos
		case WM_DEVICECHANGE:{
			std::clog<<"[evento] WM_DEVICECHANGE"<<std::endl;
			PDEV_BROADCAST_HDR lpdb=(PDEV_BROADCAST_HDR)lParam;
			switch(wParam){
				case DBT_DEVICEARRIVAL:
				case DBT_DEVICEREMOVECOMPLETE:
					std::clog<<"\tDBT_DEVICEARRIVAL || DBT_DEVICEREMOVECOMPLETE"<<std::endl;
					if(lpdb->dbch_devicetype==DBT_DEVTYP_VOLUME){
						std::clog<<"\t\tDBT_DEVTYP_VOLUME"<<std::endl;
			            dispositivos->actualizar();
			        }
			}
			break;
		}
		//Tamaño minimo de la ventana
		case WM_GETMINMAXINFO:{
			MINMAXINFO *mmi=(MINMAXINFO*)lParam;
		    mmi->ptMinTrackSize.x = 200;
		    mmi->ptMinTrackSize.y = 100;
			break;
		}
		//Desatruccion de la ventana
		case WM_DESTROY:{
			std::clog<<"[evento] WM_DESTROY"<<std::endl;
		    PostQuitMessage(0);
		    break;
		}
	}
	return salida;
}

//Main del programa
int main(int argc, char** argv) {
	std::clog<<"[main] iniciando el programa"<<std::endl;
	int salida=1;
	HWND hVentana=FindWindow(nombreClase.c_str(),NULL);
	if(hVentana){
		std::clog<<"[main] pasando el foco al programa abierto"<<std::endl;
		SetForegroundWindow(hVentana);
		salida=-1;
	}else{
		std::clog<<"[main] iniciando la ejecucion"<<std::endl;
		try{
			ControlesEspeciales(ICC_LISTVIEW_CLASSES);
			try{
				Clase clase(nombreClase,(WNDPROC)&evento);
				try{
					//creando ventana
					ventana=std::make_shared<Ventana>(clase.getClase());
					
					//iniciando menu
					mainMenu=std::make_shared<Menu>();
					
					//iniciando submenu archivos
					std::shared_ptr<Menu> fileMenu=std::make_shared<Menu>();
					fileMenu->add(ID_EXIT,"Salir");
					mainMenu->add(fileMenu,"Archivo");
					
					//iniciando submenu propiedades
					propertiesMenu=std::make_shared<Menu>();
					propertiesMenu->add(ID_RANDOM,true,"Aleatorio");
					propertiesMenu->add(ID_SEQUENTIAL,false,"Secuencial");
					mainMenu->add(propertiesMenu,"Propiedades");
					
					ventana->setMenu(mainMenu);
					
					//iniciando tabla
					tabla=std::make_shared<Tabla>(ventana);
					
					tabla->setEstilos(WS_VISIBLE|WS_CHILD|WS_TABSTOP|LVS_REPORT|LBS_NOTIFY);
					tabla->setEstilosEx(WS_EX_CLIENTEDGE);
					tabla->setEstilosTabla(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);
					tabla->setPosicion(0,0);
					
					//iniciando dispositivo
					dispositivos=std::make_shared<Dispositivos>(tabla);
					dispositivos->actualizar();
					
					//iniciando temporizador
					temporizador=std::make_shared<Temporizador>(ventana,1000);
					
					//estableciendo parametros ventana
					ventana->setTexto("Velocidad HDD");
					ventana->setEstilos(WS_OVERLAPPEDWINDOW|WS_VISIBLE);
					ventana->setTamano(700,450);
					
					
					
					bucleMensajes(ventana->get());
					salida=0;
				}catch(int e){
					error("Ventana",e);
				}
			}catch(int e){
				error("Clase",e);
			}
		}catch(int e){
			error("ControlesEspeciales",e);
		}
	}
	std::cin.get();
	return salida;
}
