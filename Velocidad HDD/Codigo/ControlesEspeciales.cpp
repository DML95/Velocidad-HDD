#include <windows.h>
#include <commctrl.h>

class ControlesEspeciales{
	private:
		INITCOMMONCONTROLSEX icex;
	public:
		ControlesEspeciales(int tipo){
			this->icex.dwSize=sizeof(INITCOMMONCONTROLSEX);
			this->icex.dwICC=tipo;
			if(!InitCommonControlsEx(&icex)){
				throw (int)GetLastError();
			}
		}
		
		int getTipo(){
			return this->icex.dwICC;
		}
};
