#ifndef CLASE_H
#define CLASE_H
	
	#include <string>
	
	//crea una clase de windows
	class Clase{
		private:
			LPCTSTR handle;
		public:
			//Constructor
			Clase(std::string nombre,const WNDPROC proc);
			//Destructor
			~Clase();
			//LPCTSTR
			LPCTSTR get();
			//getter setter Procedimento
			WNDPROC getProc();
	};

#endif
