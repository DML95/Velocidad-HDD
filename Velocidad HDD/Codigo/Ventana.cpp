#include <windows.h>

#include "Ventana.h"

HWND Ventana::crearVentana(LPCTSTR hClase,HWND padre,int estilo){
	return CreateWindowExA(
			0,
			hClase,
			0,
			estilo,
			CW_USEDEFAULT,CW_USEDEFAULT,
			CW_USEDEFAULT,CW_USEDEFAULT,
			padre,
			0,
			0,
			0
		);
}

//Constructores
Ventana::Ventana(LPCTSTR hClase,HWND padre){
	this->hVentana=crearVentana(hClase,padre,WS_CHILD);
	if(!this->hVentana){
		throw (int)GetLastError();
	}
}

Ventana::Ventana(LPCTSTR hClase){
	this->hVentana=crearVentana(hClase,0,0);
	if(!this->hVentana){
		throw (int)GetLastError();
	}
}

Ventana::Ventana(HWND hVentana){
	if(IsWindow(hVentana)){
		this->hVentana=hVentana;
	}else{
		throw (int)GetLastError();
	}
}

Ventana::Ventana(){
	this->hVentana=0;
}

//Destructor
Ventana::~Ventana(){
	DestroyWindow(this->hVentana);
}

//Getter HWND ventana
HWND Ventana::getVentana(){
	return this->hVentana;
}

//Getter y setter estilos
int Ventana::setEstilos(int estilos){
	estilos=SetWindowLong(this->hVentana,GWL_STYLE,estilos);
	return estilos;
}

int Ventana::getEstilos(){
	return GetWindowLong(this->hVentana,GWL_STYLE);
}

//Getter y setter estilos extendidos
int Ventana::setEstilosEx(int estilos){
	estilos=SetWindowLong(this->hVentana,GWL_EXSTYLE,estilos);
	return estilos;
}

int Ventana::getEstilosEx(){
	return GetWindowLong(this->hVentana,GWL_EXSTYLE);
}

//Getter y setter tamaño
void Ventana::setTamano(int anchura,int altura){
	SetWindowPos(this->hVentana,0,0,0,anchura,altura,SWP_NOMOVE|SWP_NOZORDER);
}

//Getter y setter posicion
void Ventana::setPosicion(int x,int y){
	SetWindowPos(this->hVentana,0,x,y,0,0,SWP_NOSIZE|SWP_NOZORDER);
}

//Getter setter texto
void Ventana::setTexto(const char *texto){
	SetWindowText(this->hVentana,texto);
}

//Getter setter Padre
void Ventana::setPadre(Ventana *padre){
	SetParent(this->hVentana,padre->getVentana());
}

void Ventana::setPadre(HWND hVentana){
	SetParent(this->hVentana,hVentana);
}

//getter setter color
void Ventana::setColor(int color){
	color&=0xffffff;
	SetClassLongPtr(this->hVentana,GCLP_HBRBACKGROUND,(long long)CreateSolidBrush(color));
}

//Repinta la ventana
void Ventana::repintar(){
	SetWindowPos(this->hVentana,0,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER|SWP_FRAMECHANGED|SWP_NOCOPYBITS);
}

//destruye la ventana
void Ventana::destruir(){
	DestroyWindow(this->hVentana);
}
