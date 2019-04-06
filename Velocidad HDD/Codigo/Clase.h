#include <string>

//crea una clase de windows
class Clase{
	private:
		LPCTSTR hClase;
	public:
		//Constructor
		Clase(std::string nombre,const WNDPROC proc);
		//Destructor
		~Clase();
		//LPCTSTR
		LPCTSTR getClase();
		//getter setter Procedimento
		WNDPROC getProc();
		void setProc(WNDPROC proc);
		HWND getHVentana();
};
