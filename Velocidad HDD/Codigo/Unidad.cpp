#include <windows.h>

class Unidad{
	private:
		HANDLE hUnidad;
		int byteSector;
		long long numSector;
	public:
		
		//Constructor
		Unidad(char unidad){
			int tam;
			DISK_GEOMETRY disco;
			char nombreUnidad[]="\\\\.\\ :";
			nombreUnidad[4]=unidad;
			this->hUnidad=CreateFile(
					nombreUnidad,
					GENERIC_READ,
					FILE_SHARE_READ|FILE_SHARE_WRITE,
					0,
					OPEN_EXISTING,
					FILE_ATTRIBUTE_NORMAL|FILE_FLAG_NO_BUFFERING,
					0);
			if(this->hUnidad==INVALID_HANDLE_VALUE){
				throw (int)GetLastError();
			}else{
				if(DeviceIoControl(this->hUnidad,IOCTL_DISK_GET_DRIVE_GEOMETRY,0,0,(LPVOID)&disco,sizeof(DISK_GEOMETRY),(LPDWORD)&tam,0)){
					this->byteSector=disco.BytesPerSector;
					this->numSector=(long long)disco.Cylinders.QuadPart*(long long)disco.TracksPerCylinder*(long long)disco.SectorsPerTrack;
				}else{
					CloseHandle(this->hUnidad);
					throw (int)GetLastError();
				}
			}
		}
		
		//Destructor
		~Unidad(){
			CloseHandle(this->hUnidad);
		}
		
		//getter Setter
		int getByteSector(){
			return this->byteSector;
		}
		
		void getNumSector(long long *numSector){
			*numSector=this->numSector;
		}
		
		bool getRegistro(char *datos,long long numSector){
			int tam,*registro=(int*)&numSector;
			bool salida=SetFilePointer(this->hUnidad,(LONG)*registro,(PLONG)*(registro+1),FILE_BEGIN);
			if(salida)salida=ReadFile(this->hUnidad,datos,this->byteSector,(LPDWORD)&tam,0);
			return salida;
		}
		
		void cancelarOperacion(){
			CancelIo(this->hUnidad);
		}
};
