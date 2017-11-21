#include <windows.h>

class Ventana{
	protected:
		HWND hVentana;
	private:
		//Crea la ventana
		HWND crearVentana(LPCTSTR hClase,HWND padre,int estilo){
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
	public:
		
		//Constructores
		Ventana(LPCTSTR hClase,HWND padre){
			this->hVentana=crearVentana(hClase,padre,WS_CHILD);
			if(!this->hVentana){
				throw (int)GetLastError();
			}
		}
		
		Ventana(LPCTSTR hClase){
			this->hVentana=crearVentana(hClase,0,0);
			if(!this->hVentana){
				throw (int)GetLastError();
			}
		}
		
		Ventana(HWND hVentana){
			if(IsWindow(hVentana)){
				this->hVentana=hVentana;
			}else{
				throw (int)GetLastError();
			}
		}
		
		Ventana(){
			this->hVentana=0;
		}
		
		//Destructor
		~Ventana(){
			DestroyWindow(this->hVentana);
		}
		
		//Getter HWND ventana
		HWND getVentana(){
			return this->hVentana;
		}
		
		//Getter y setter estilos
		int setEstilos(int estilos){
			estilos=SetWindowLong(this->hVentana,GWL_STYLE,estilos);
			return estilos;
		}
		
		int getEstilos(){
			return GetWindowLong(this->hVentana,GWL_STYLE);
		}
		
		//Getter y setter estilos extendidos
		int setEstilosEx(int estilos){
			estilos=SetWindowLong(this->hVentana,GWL_EXSTYLE,estilos);
			return estilos;
		}
		
		int getEstilosEx(){
			return GetWindowLong(this->hVentana,GWL_EXSTYLE);
		}
		
		//Getter y setter tamaño
		void setTamano(int anchura,int altura){
			SetWindowPos(this->hVentana,0,0,0,anchura,altura,SWP_NOMOVE|SWP_NOZORDER);
		}
		
		//Getter y setter posicion
		void setPosicion(int x,int y){
			SetWindowPos(this->hVentana,0,x,y,0,0,SWP_NOSIZE|SWP_NOZORDER);
		}
		
		//Getter setter texto
		void setTexto(const char *texto){
			SetWindowText(this->hVentana,texto);
		}
		
		//Getter setter Padre
		void setPadre(Ventana *padre){
			SetParent(this->hVentana,padre->getVentana());
		}
		
		void setPadre(HWND hVentana){
			SetParent(this->hVentana,hVentana);
		}
		
		//getter setter color
		void setColor(int color){
			color&=0xffffff;
			SetClassLong(this->hVentana,GCL_HBRBACKGROUND,(int)CreateSolidBrush(color));
		}
		
		//Repinta la ventana
		void repintar(){
			SetWindowPos(this->hVentana,0,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER|SWP_FRAMECHANGED|SWP_NOCOPYBITS);
		}
		
		//destruye la ventana
		void destruir(){
			DestroyWindow(this->hVentana);
		}
		
};
