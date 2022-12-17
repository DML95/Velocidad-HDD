#ifndef DEVICEMEASURING_H
#define DEVICEMEASURING_H

	#include <atomic>
	#include <memory>
	#include <string>
	#include <thread>

	#include "winapi.h"

	#include "deviceInfo.h"
	#include "random.h"

	//inicia la medicion
	class DeviceMeasuring{
		private:

			typedef struct{
				unsigned long long sector;
				unsigned int sizeBlock;
			}InfoRead;

            typedef void (*getSector)(InfoRead&,DeviceMeasuring&);

			std::shared_ptr<DeviceInfo> deviceInfo;
			std::atomic_bool run;
			std::atomic<HANDLE> handle;
			std::atomic_int operations;
			std::atomic_int errors;
			std::atomic<void*> getSectorPtr;
			Random randomGen;
			//std::atomic_uint sizeBlock;

			bool (*errorAsync)(DeviceMeasuring&,int,void*);
			void *paramError;
			HANDLE thread;
			unsigned int sizeSector;
			unsigned long long numberSectors;
			unsigned int sequentialSizeBlock;

			//lee de manera asincrona un registro
			static bool readSector(std::vector<char> &buffer,OVERLAPPED *overlapped,DeviceMeasuring &deviceMeasuring,InfoRead &infoRead);
			//hilo en segundo plano que realiza la medicion
			static void mainThread(DeviceMeasuring &deviceMeasuring);

			//funcion que devuelave un sector aleatorio
			static void getRandomSector(InfoRead &infoRead,DeviceMeasuring &deviceMeasuring);
			//funcion que devuelve un sector secuencial
			static void getSequentialSector(InfoRead &infoRead,DeviceMeasuring &deviceMeasuring);

			//abre el dispositivo
			void init(std::string &device);
			//lee la informacion de la extructura logica del disco
			void readGeometryInfo();
			//lee la informacion del tamaño fisico de los sectores
			//llamar siempre despues de readGeometryInfo()
			void readAccessAlignmentInfo();
			//optiene un numero de sectores mas grande para la lectura secuancial
			//en las lecturas secuanciales se tiene que leer un bloque de sectores para saturar el disco
			void getSequentialSizeBlock();

		public:

			typedef struct{
				long long operations;
				long long bytes;
				long long errors;
			}measuring;

			enum mode{
				random,
				sequential
			};

			//constructor
			DeviceMeasuring(std::shared_ptr<DeviceInfo> &deviceInfo,DeviceMeasuring::mode mode,bool (*errorAsync)(DeviceMeasuring&,int,void*),void *paramError);
			//destructor
			~DeviceMeasuring();
			//optiene la medicion y reinia el contador
			void getMeasuringAndReset(DeviceMeasuring::measuring &measuring);
			//establece el modo de lectura
			void setMode(DeviceMeasuring::mode mode);
			//devualve el tam�o del dispostivo
			long long getSize();
	};

#endif
