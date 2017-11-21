#include <windows.h>

class Clase{
	private:
		LPCTSTR hClase;
	public:
		//Constructor
		Clase(char *nombre,const WNDPROC proc){
			ATOM atomo;
			WNDCLASSEX dClase;
			dClase.cbSize=sizeof(WNDCLASSEX);
			dClase.style=0;
			dClase.lpfnWndProc=proc;
			dClase.cbClsExtra=0;
			dClase.cbWndExtra=0;
			dClase.hInstance=0;
			dClase.hIcon=0;
			dClase.hCursor=0;
			dClase.hbrBackground=(HBRUSH)(COLOR_MENU+1);
			dClase.lpszMenuName=0;
			dClase.lpszClassName=nombre;
			dClase.hIconSm=0;
			atomo=RegisterClassEx(&dClase);
			if(!atomo){
				throw (int)GetLastError();
			}
			this->hClase=MAKEINTATOM(atomo);
		}
		
		//Destructor
		~Clase(){
			UnregisterClass(this->hClase,0);
		}
		
		//LPCTSTR
		LPCTSTR getClase(){
			return this->hClase;
		}
		
		//getter setter Procedimento
		WNDPROC getProc(){
			WNDCLASSEX dClase;
			GetClassInfoEx(0,this->hClase,&dClase);
			return dClase.lpfnWndProc;
		}
		
		void setProc(WNDPROC proc){
			WNDCLASSEX dClase;
			GetClassInfoEx(0,this->hClase,&dClase);
			dClase.lpfnWndProc=proc;
		}
		
		//
		HWND getHVentana(){
			return FindWindow(this->hClase,NULL);
		}
};
