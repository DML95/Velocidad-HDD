#ifndef MAIN_H
#define MAIN_H
	
	//inicio del programa
	static const std::string nombreClase="VelocidadHDD";
	
	static void error(std::string error,int num);
	
	//Arranca el bucle de mensajes
	static int bucleMensajes(HWND hVentana);
	//Funcion de recepcion de Eventos
	static WNDPROC evento(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	
	int main(int argc, char** argv);

#endif
