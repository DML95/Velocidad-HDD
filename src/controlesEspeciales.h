#ifndef CONTROLESESPECIALES_H
#define CONTROLESESPECIALES_H

    #include "winapi.h"

	//inicia los controles especiales de windows (tabla, etc)
	class ControlesEspeciales{
		private:
			INITCOMMONCONTROLSEX icex;
		public:
			ControlesEspeciales(int tipo);

			int getTipo();
	};

#endif
