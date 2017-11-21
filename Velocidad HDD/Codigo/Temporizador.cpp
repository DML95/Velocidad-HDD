#include <windows.h>

class Temporizador{
	private:
		UINT_PTR hTemporizador;
	public:
		//Constructor
		Temporizador(HWND hVentana,int tiempo){
			this->hTemporizador=SetTimer(hVentana,0,tiempo,0);
			if(!this->hTemporizador){
				throw (int)GetLastError();
			}
		}
		
		//Destructor
		~Temporizador(){
			KillTimer(0,this->hTemporizador);
		}
};
