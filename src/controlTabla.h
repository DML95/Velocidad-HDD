#ifndef CONTROLTABLA_H
#define CONTROLTABLA_H

	#include<vector>

	#include "winapi.h"

	#include "tabla.h"
	#include "ventana.h"
	#include "deviceInfo.h"
	#include "deviceMeasuring.h"

	//controlador de la tabla de windows
	class ControlTabla{
		private:

			enum columna{
				nombre,
				unidades,
				capacidad,
				bytesSegundo,
				iops,
				porcentajeErrores,
				error
			};

			typedef struct{
				std::string name;
				char id;
				bool namePhysical;	//es un dispositivo fisico
				std::shared_ptr<DeviceInfo> deviceInfo;
				std::shared_ptr<DeviceMeasuring> deviceMeasuring;
			}device;

			std::vector<ControlTabla::device> devices;
			const std::shared_ptr<Tabla> tabla;
			const std::shared_ptr<Ventana> ventana;
			DeviceMeasuring::mode mode;

			//recorta el nuemro de decimales de un float
			static std::string toString(float number,int decimalNumber);
			//devuevelve un string con las unidades correctas B/s, KB/s, ...
			static std::string adjustmentUnit(long long bytes);
			//agrega las columnas a la tabla
			void addColumn();
			//rellena la lista de devices
			void addDevices();
			//actualiza la informacion de las paticiones
			void updatePartitionsUnit(device &device,std::shared_ptr<DeviceInfo> &newDeviceInfo,int fila);
			//elimina, agrega, cuenta y actualiza una fila de la tabla
			void modifiyTable(device &device,std::shared_ptr<DeviceInfo> &newDeviceInfo,int &contFila);

			//void agregarDispositivo(char dispositivo,int posicion);

			//void eliminarDispositivo(int dispositivo,int posicion);

			//devuelve el dispositivo a partir de la fila de la tabla
			ControlTabla::device* getDevice(int fila);
			//funcion llamada cuando se genera un error en alguna medicion
			static bool errorAsync(DeviceMeasuring &deviceMeasuring,int error,void *param);

		public:

			static constexpr int WM_FORCEENDMEASURING=WM_USER+1;

			//Constructor
			ControlTabla(std::shared_ptr<Tabla> &tabla,std::shared_ptr<Ventana> &ventana);
			//Destructor
			~ControlTabla();
			//Actualiza la tabla
			void updateDevice();
			//Inicia o detiene la medicion de un dispositivo
			void setMeasuring(int fila,bool run);
			//Mustra la medicion en la tabla
			void displayMeasuring();
			//establece el modo de lectura
			void setMode(DeviceMeasuring::mode mode);
			//fuerza la detencion de medida
			void forceEndMeasuring(int fila,int error);
	};

#endif
