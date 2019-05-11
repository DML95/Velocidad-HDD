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

static std::shared_ptr<Ventana> ventana;
static std::shared_ptr<Tabla> tabla;
static std::shared_ptr<Dispositivos> dispositivos;
static std::shared_ptr<Temporizador> temporizador;

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
		//Creacion de la ventana
		case WM_CREATE:{
			std::clog<<"[evento] WM_CREATE"<<std::endl;
			try{
				tabla=std::make_shared<Tabla>(hWnd);
				
				tabla->setEstilos(WS_VISIBLE|WS_CHILD|WS_TABSTOP|LVS_REPORT|LBS_NOTIFY);
				tabla->setEstilosEx(WS_EX_CLIENTEDGE);
				tabla->setEstilosTabla(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);
				tabla->setPosicion(0,0);
				
				dispositivos=std::make_shared<Dispositivos>(tabla);
				dispositivos->actualizar();
				temporizador=std::make_shared<Temporizador>(hWnd,1000);
			}catch(int e){
				DestroyWindow(hWnd);
				error("Tabla",e);
			}
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
					ventana=std::make_shared<Ventana>(clase.getClase());
					
					ventana->setEstilos(WS_OVERLAPPEDWINDOW|WS_VISIBLE);
					ventana->setTamano(700,400);
					ventana->setTexto("Velocidad HDD");
					bucleMensajes(ventana->getVentana());
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
