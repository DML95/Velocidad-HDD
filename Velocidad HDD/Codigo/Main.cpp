#include<cstdio>
#include<iostream>
#include <windows.h>
#include <dbt.h>
#include <commctrl.h>

#include "Temporizador.h"
#include "ControlesEspeciales.h"
#include "Dispositivos.h"
#include "Clase.h"
#include "Main.h"


static void error(char *error,int num){
	char texto[0xff];
	std::sprintf(texto,"Error clase '%s'\n\nError numero %i",error,num);
	MessageBox(0,texto,"Error",0);
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
	int salida=0;
	switch(msg){
		//Redimension de la ventana
		case WM_SIZE:
			pTabla->setTamano(LOWORD(lParam),HIWORD(lParam));
			break;
		//Evento temporizador
		case WM_TIMER:
			std::srand(std::rand());
			pDispositivos->mostrarMedicion();
			break;
		//Notificaciones de la tabla
		case WM_NOTIFY:{
			NMHDR *header = (NMHDR*)lParam;
	        NMLISTVIEW *nmlist = (NMLISTVIEW*)lParam;
	        switch(header->code){
	        	case LVN_ITEMCHANGED:
	        		switch(nmlist->uNewState&LVIS_STATEIMAGEMASK){
	        			case INDEXTOSTATEIMAGEMASK(BST_CHECKED+1):
	        				pDispositivos->iniciar(nmlist->iItem);
	        				
	        				break;
	        			case INDEXTOSTATEIMAGEMASK(BST_UNCHECKED+1):
	        				pDispositivos->finalizar(nmlist->iItem);
					}
			}
	        break;
		}
		//Cambio de dispositivos
		case WM_DEVICECHANGE:{
			PDEV_BROADCAST_HDR lpdb=(PDEV_BROADCAST_HDR)lParam;
			switch(wParam){
				case DBT_DEVICEARRIVAL:
				case DBT_DEVICEREMOVECOMPLETE:
					if(lpdb->dbch_devicetype==DBT_DEVTYP_VOLUME){
			            pDispositivos->actualizar();
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
			try{
				pTabla=new Tabla(hWnd);
				
				pTabla->setEstilos(WS_VISIBLE|WS_CHILD|WS_TABSTOP|LVS_REPORT|LBS_NOTIFY);
				pTabla->setEstilosEx(WS_EX_CLIENTEDGE);
				pTabla->setEstilosTabla(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);
				pTabla->setPosicion(0,0);
				
				pDispositivos=new Dispositivos(pTabla);
				pDispositivos->actualizar();
				pTemporizador=new Temporizador(hWnd,1000);
			}catch(int e){
				DestroyWindow(hWnd);
				error("Tabla",e);
			}
			break;
		}
		//Desatruccion de la ventana
		case WM_DESTROY:
			delete pTemporizador;
			delete pTabla;
			delete pDispositivos;
		    PostQuitMessage(0);
		    break;
		//Eventos no procesados
		default:
			salida=DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return (WNDPROC)salida;
}

//Main del programa
int main(int argc, char** argv) {
	int salida=1;
	HWND hVentana=FindWindow(nombreClase,NULL);
	if(hVentana){
		SetForegroundWindow(hVentana);
		salida=-1;
	}else{
		try{
			ControlesEspeciales(ICC_LISTVIEW_CLASSES);
			try{		Clase clase=Clase((char*)nombreClase,(WNDPROC)&evento);
				try{
					pVentana=new Ventana(clase.getClase());
					
					pVentana->setEstilos(WS_OVERLAPPEDWINDOW|WS_VISIBLE);
					pVentana->setTamano(600,400);
					pVentana->setTexto("Velocidad HDD");						
					//pVentana->repintar();
					bucleMensajes(pVentana->getVentana());
					delete pVentana;
					salida=0;
				}catch(int e){
					error("Ventana",e);
				}
				clase.~Clase();
			}catch(int e){
				error("Clase",e);
			}
		}catch(int e){
			error("ControlesEspeciales",e);
		}
	}
	return salida;
}
