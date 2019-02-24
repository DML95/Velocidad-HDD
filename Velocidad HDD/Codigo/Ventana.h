#include <windows.h>

//crea una ventana de windows
class Ventana{
	protected:
		HWND hVentana;
	private:
		//Crea la ventana
		HWND crearVentana(LPCTSTR hClase,HWND padre,int estilo);
		
	public:
		
		//Constructores
		Ventana(LPCTSTR hClase,HWND padre);
		
		Ventana(LPCTSTR hClase);
		
		Ventana(HWND hVentana);
		
		Ventana();
		
		//Destructor
		~Ventana();
		
		//Getter HWND ventana
		HWND getVentana();
		
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
		
		//Getter setter texto
		void setTexto(const char *texto);
		
		//Getter setter Padre
		void setPadre(Ventana *padre);
		
		void setPadre(HWND hVentana);
		
		//getter setter color
		void setColor(int color);
		
		//Repinta la ventana
		void repintar();
		
		//destruye la ventana
		void destruir();
		
};
