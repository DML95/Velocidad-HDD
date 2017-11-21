#include <iostream>
#include <windows.h>
#include "Unidad.cpp"

class Medicion{
	private:
		bool finHilo;
		int operaciones,error;
		char dispositivo;
		Unidad *unidad;
		HANDLE hHilo;
		
		static int hilo(Medicion *medicion){
			long long sectorAleatorio,numSectores;
			int tamanoSector=medicion->unidad->getByteSector();
			char buffer[2*tamanoSector-1];
			char *bufferAlineado=(char*)((int)(buffer+tamanoSector-1)&~(tamanoSector-1));
			medicion->unidad->getNumSector(&numSectores);
			while(medicion->finHilo){
				longLongAleatorio(&sectorAleatorio,numSectores);
				if(medicion->unidad->getRegistro(bufferAlineado,sectorAleatorio)){
					medicion->operaciones++;
				}else medicion->error++;
			}
			return 0;
		}
		
		static void longLongAleatorio(long long *valor,long long maximo){
			int cont;
			short *subValor=(short*)valor;
			for(cont=0;cont<4;cont++){
				subValor[cont]=(short)std::rand();
			}
			*valor%=maximo;
		}
		
	public:
		//Constructor
		Medicion(){
			this->hHilo=0;
			this->finHilo=false;
			this->dispositivo=0;
			this->unidad=0;
			this->operaciones=-1;
			this->error=-1;
		}
		
		//Destructor
		~Medicion(){
			detener();
		}
		
		//Getter y setter
		void iniciar(){
			long long numSector;
			if(!this->hHilo){
				this->finHilo=true;
				this->unidad=new Unidad(this->dispositivo);
				this->unidad->getNumSector(&numSector);
				this->hHilo=CreateThread(0,0,(LPTHREAD_START_ROUTINE)hilo,this,0,0);
				this->operaciones=0;
				this->error=0;
			}
		}
		
		void detener(){
			if(this->hHilo){
				this->finHilo=false;
				this->unidad->cancelarOperacion();
				WaitForSingleObject(this->hHilo,INFINITE);
				CloseHandle(this->hHilo);
				delete this->unidad;
				this->unidad=0;
				this->hHilo=0;
				this->operaciones=-1;
				this->error=-1;
			}
		}
		
		char getDispositivo(){
			return this->dispositivo;
		}
		
		int getOperaciones(int *error){
			int operaciones=this->operaciones;
			*error=this->error;
			if(operaciones>=0){
				this->operaciones=0;
				this->error=0;
			}
			return operaciones;
		}
		
		int getByteSector(){
			return this->unidad->getByteSector();
		}
		
		void setDispositivo(char dispositivo){
			this->dispositivo=dispositivo;
		}
};
