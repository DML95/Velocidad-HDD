#include <windows.h>
#include<iostream>

#include "Unidad.h"

Unidad::Unidad(char unidad){
	std::clog<<"[Unidad] iniciando unidad: "<<unidad<<std::endl;
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
	std::clog<<"[Unidad] unidad info\n\tbytes por sector: "<<this->byteSector<<
			"\n\tnumero de sectores: "<<this->numSector<<std::endl;
}

//Destructor
Unidad::~Unidad(){
	std::clog<<"[Unidad] eliminado unidad"<<std::endl;
	CloseHandle(this->hUnidad);
}

//getter Setter
int Unidad::getByteSector(){
	return this->byteSector;
}

long long Unidad::getNumSector(){
	return this->numSector;
}

bool Unidad::getRegistro(char *datos,long long numSector){
	long long tam,*registro=(long long*)&numSector;
	bool salida=SetFilePointer(this->hUnidad,(LONG)*registro,(PLONG)*(registro+1),FILE_BEGIN);
	if(salida)salida=ReadFile(this->hUnidad,datos,this->byteSector,(LPDWORD)&tam,0);
	return salida;
}

void Unidad::cancelarOperacion(){
	CancelIo(this->hUnidad);
}
