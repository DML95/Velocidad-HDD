#include <windows.h>
#include "Ventana.cpp"

#include <commctrl.h>

class Tabla:public Ventana{
	private:
		
	public:
		
		//Constructor
		Tabla(HWND padre):Ventana(WC_LISTVIEW,padre){
			
		}
		
		//Columnas
		void agregarColumna(char *texto,int anchuraPx,int posicion){
			LVCOLUMN datosColumna;
			datosColumna.mask=LVCF_FMT|LVCF_TEXT|LVCF_WIDTH;
			datosColumna.fmt=LVCFMT_LEFT;
			datosColumna.cx=anchuraPx;
			datosColumna.pszText=texto;
			SendMessage(this->hVentana,LVM_INSERTCOLUMN,posicion,(LPARAM)&datosColumna);
		}
		
		//Filas
		void agregarFilas(int numFilas,int posicion){
			int cont;
			LVITEM datosFila;
			ZeroMemory(&datosFila,sizeof(LVITEM));
			datosFila.iItem=posicion;
			for(cont=0;cont<numFilas;cont++){
				SendMessage(this->hVentana,LVM_INSERTITEM,0,(LPARAM)&datosFila);
			}
		}
		
		void eliminarFilas(int numFilas,int posicion){
			int cont;
			for(cont=0;cont<numFilas;cont++){
				SendMessage(this->hVentana,LVM_DELETEITEM,posicion,0);
			}
		}
		
		int getNumFilas(){
			return SendMessage(this->hVentana,LVM_GETITEMCOUNT,0,0);
		}
		
		//Celdas
		void setValor(char *texto,int fila,int columna){
			LVITEM datosFila;
			datosFila.pszText=texto;
			datosFila.iSubItem=columna;
			SendMessage(this->hVentana,LVM_SETITEMTEXT,fila,(LPARAM)&datosFila);
		}
		
		int getValor(char *texto,int tamano,int fila,int columna){
			LVITEM datosFila;
			datosFila.pszText=texto;
			datosFila.iSubItem=columna;
			datosFila.cchTextMax=tamano;
			return SendMessage(this->hVentana,LVM_GETITEMTEXT,fila,(LPARAM)&datosFila);
		}
		
		//checkbox
		void setChecked(int fila,bool checked){
			LVITEM datosFila;
			datosFila.state=checked?INDEXTOSTATEIMAGEMASK(2):INDEXTOSTATEIMAGEMASK(1);
			datosFila.stateMask=LVIS_STATEIMAGEMASK;
			SendMessage(this->hVentana,LVM_SETITEMSTATE,fila,(LPARAM)&datosFila);
		}
		
		//Estilo
		void setEstilosTabla(int estilos){
			SendMessage(this->hVentana,LVM_SETEXTENDEDLISTVIEWSTYLE,0,estilos);
		}
};
