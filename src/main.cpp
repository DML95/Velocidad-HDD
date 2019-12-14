#include<iostream>

#include "main.h"

const std::string Main::nameClass="VelocidadHDD";
std::shared_ptr<Main> Main::main(nullptr);

void Main::messageBoxError(std::string error,int num){
	std::string texto=error+"\n\nError numero "+std::to_string(num);
	std::clog<<"[Error] "<<texto<<std::endl;
	MessageBox(0,texto.c_str(),"Error",0);
}

WNDPROC Main::event(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
	WNDPROC out=(WNDPROC)DefWindowProc(hWnd, msg, wParam, lParam);
	switch(msg){
		//Redimension de la ventana
		case WM_SIZE:{
			std::clog<<"[Main::evento] WM_SIZE"<<std::endl;
			std::shared_ptr<Main> mainInstance=Main::getInstance();
			mainInstance->tabla->setTamano(LOWORD(lParam),HIWORD(lParam));
			break;
		}
		//Evento temporizador
		case WM_TIMER:{
			std::srand(std::rand());
			std::shared_ptr<Main> mainInstance=Main::getInstance();
			mainInstance->controlTabla->displayMeasuring();
			break;
		}
		//Notificaciones de la tabla
		case WM_NOTIFY:{
			NMHDR *header = (NMHDR*)lParam;
	        NMLISTVIEW *nmlist = (NMLISTVIEW*)lParam;
	        switch(header->code){
	        	case LVN_ITEMCHANGED:
	        		std::clog<<"[Main::evento] WM_NOTIFY\n\tLVN_ITEMCHANGED"<<std::endl;
					std::shared_ptr<Main> mainInstance=Main::getInstance();
	        		switch(nmlist->uNewState&LVIS_STATEIMAGEMASK){
	        			case INDEXTOSTATEIMAGEMASK(BST_CHECKED+1):
	        				mainInstance->controlTabla->setMeasuring(nmlist->iItem,true);
	        				break;
	        			case INDEXTOSTATEIMAGEMASK(BST_UNCHECKED+1):
	        				mainInstance->controlTabla->setMeasuring(nmlist->iItem,false);
					}
			}
	        break;
		}
		//eventos del menu
		case WM_COMMAND:{
			std::clog<<"[Main::evento] WM_COMMAND"<<std::endl;
			std::shared_ptr<Main> mainInstance=Main::getInstance();
			switch(LOWORD(wParam)){
				case Main::idMenu::exit:
					mainInstance->ventana->destruir();
					break;
				case Main::idMenu::random:
					mainInstance->propertiesMenu->setChack(Main::idMenu::random,true);
					mainInstance->propertiesMenu->setChack(Main::idMenu::sequential,false);
					mainInstance->controlTabla->setMode(DeviceMeasuring::mode::random);
					break;
				case Main::idMenu::sequential:
					mainInstance->propertiesMenu->setChack(Main::idMenu::sequential,true);
					mainInstance->propertiesMenu->setChack(Main::idMenu::random,false);
					mainInstance->controlTabla->setMode(DeviceMeasuring::mode::sequential);
					break;
			}
            break;
        }
		//Cambio de dispositivos
		case WM_DEVICECHANGE:{
			std::clog<<"[Main::evento] WM_DEVICECHANGE"<<std::endl;
			PDEV_BROADCAST_HDR lpdb=(PDEV_BROADCAST_HDR)lParam;
			switch(wParam){
				case DBT_DEVNODES_CHANGED:
					std::clog<<"\tDBT_DEVNODES_CHANGED"<<std::endl;
					std::shared_ptr<Main> mainInstance=Main::getInstance();
					mainInstance->controlTabla->updateDevice();
			}
			break;
		}
		case ControlTabla::WM_FORCEENDMEASURING:{
			std::clog<<"[Main::evento] WM_FORCEENDMEASURING"<<std::endl;
			std::shared_ptr<Main> mainInstance=Main::getInstance();
			mainInstance->controlTabla->forceEndMeasuring(wParam,lParam);
			break;
		}
		//Tamaño minimo de la ventana
		case WM_GETMINMAXINFO:{
			MINMAXINFO *mmi=(MINMAXINFO*)lParam;
		    mmi->ptMinTrackSize.x = 500;
		    mmi->ptMinTrackSize.y = 300;
			break;
		}
		//Destruccion de la ventana
		case WM_DESTROY:{
			std::clog<<"[Main::evento] WM_DESTROY"<<std::endl;
		    PostQuitMessage(0);
		    break;
		}
	}
	return out;
}

Main::Main(){
	std::clog<<"[Main] Iniciando programa"<<std::endl;
	int errorNumber=-1;
	try{
		ControlesEspeciales(ICC_LISTVIEW_CLASSES);
		try{
			this->clase=std::make_shared<Clase>(Main::nameClass,(WNDPROC)&Main::event);
			try{
				//iniciando menu
				this->mainMenu=std::make_shared<Menu>();
				
				//iniciando submenu archivos
				this->fileMenu=std::make_shared<Menu>();
				this->fileMenu->add(Main::idMenu::exit,"Salir");
				this->mainMenu->add(this->fileMenu,"Archivo");
				
				//iniciando submenu propiedades
				this->propertiesMenu=std::make_shared<Menu>();
				this->propertiesMenu->add(Main::idMenu::random,true,"Aleatorio");
				this->propertiesMenu->add(Main::idMenu::sequential,false,"Secuencial");
				this->mainMenu->add(this->propertiesMenu,"Propiedades");
				
			}catch(int e){
				Main::messageBoxError("Error en incio del nucleo",e);
				errorNumber=e;
			}
		}catch(int e){
			Main::messageBoxError("Error al crear clase",e);
			errorNumber=e;
		}
	}catch(int e){
		Main::messageBoxError("Error al iniciar los controles especiales",e);
		errorNumber=e;
	}
	if(errorNumber>=0){
		throw errorNumber;
	}
}

int Main::messageLoop(){
	std::clog<<"[Main] Iniciando bucle de mensajes"<<std::endl;
	MSG msg;
	HWND hwnd=Main::ventana->get();
	while(GetMessage(&msg,hwnd,0,0)>0){
		if(!IsDialogMessage(hwnd,&msg)){
		    TranslateMessage(&msg);
		    DispatchMessage(&msg);
		}
	}
	return msg.lParam;
}

int Main::run(){
	std::clog<<"[Main] Arrancando programa"<<std::endl;
	int out;
	try{
		//creando ventana
		this->ventana=std::make_shared<Ventana>(this->clase);
		
		//agregando menu archivos
		this->ventana->setMenu(this->mainMenu);
		
		//iniciando tabla
		this->tabla=std::make_shared<Tabla>((std::shared_ptr<VentanaAbstract>&)this->ventana);
		
		this->tabla->setEstilos(WS_VISIBLE|WS_CHILD|WS_TABSTOP|LVS_REPORT|LBS_NOTIFY);
		this->tabla->setEstilosEx(WS_EX_CLIENTEDGE);
		this->tabla->setEstilosTabla(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);
		this->tabla->setPosicion(0,0);
		
		//iniciando dispositivo
		this->controlTabla=std::make_shared<ControlTabla>(this->tabla,this->ventana);
		this->controlTabla->updateDevice();
		
		//iniciando temporizador
		Main::temporizador=std::make_shared<Temporizador>(this->ventana,1000);
		
		//estableciendo parametros ventana
		this->ventana->setTexto("Velocidad HDD");
		this->ventana->setEstilos(WS_OVERLAPPEDWINDOW|WS_VISIBLE);
		this->ventana->setTamano(1200,450);
		
		//iniciamos el bucle de mensajes
		out=this->messageLoop();
	}catch(int e){
		Main::messageBoxError("Al arrancar programa",e);
		out=1;
	}
	return out;
}

Main::~Main(){
	std::clog<<"[Main] Cerrando programa"<<std::endl;
}

std::shared_ptr<Main>& Main::getInstance(){
	if(!Main::main){
		HWND hVentana=FindWindow(Main::nameClass.c_str(),NULL);
		//comprovamos si el programa ya esta ejecutandose
		if(hVentana){
			std::clog<<"[Main] Pasando el foco al programa abierto"<<std::endl;
			SetForegroundWindow(hVentana);
		}else{
			std::clog<<"[Main] Creando instancia: "<<(bool)Main::main<<std::endl;
			try{
				Main::main.reset(new Main());
			}catch(int e){
				
			}
		}
	}
	return Main::main;
}

int main(){
	int out=1;
	std::shared_ptr<Main> mainInstance=Main::getInstance();
	if(mainInstance){
		out=mainInstance->run();
	}
	std::cin.get();
	return out;
}
