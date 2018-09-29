class Clase{
	private:
		LPCTSTR hClase;
	public:
		//Constructor
		Clase(char *nombre,const WNDPROC proc);
		//Destructor
		~Clase();
		//LPCTSTR
		LPCTSTR getClase();
		//getter setter Procedimento
		WNDPROC getProc();
		void setProc(WNDPROC proc);
		HWND getHVentana();
};
