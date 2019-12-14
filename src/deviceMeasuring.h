#ifndef DEVICEMEASURING_H
#define DEVICEMEASURING_H
	
	#include <atomic>
	#include <memory>
	#include <string>
	#include <thread>
	#include <windows.h>
	
	#include "deviceInfo.h"
	#include "wait.h"
	
	//inicia la medicion
	class DeviceMeasuring{
		private:
			
			std::shared_ptr<DeviceInfo> deviceInfo;
			std::atomic_bool run;
			std::atomic<HANDLE> handle;
			std::atomic_int operations;
			std::atomic_int errors;
			std::atomic<void(*)(long long&,long long)> getSector;
			Wait wait;
			
			bool (*errorAsync)(DeviceMeasuring*,int,void*);
			void *paramError;
			std::thread thread;
			unsigned int sizeSector;
			unsigned long long numberSectors;
			
			//lee de manera asincrona un registro
			static bool readSector(std::vector<char> &buffer,OVERLAPPED *overlapped,DeviceMeasuring *deviceMeasuring);
			//hilo en segundo plano que realiza la medicion
			static void mainThread(DeviceMeasuring *deviceMeasuring);
			
			//funcion que devulave un sector aleatorio
			static void getRandonSector(long long &selectSector,long long numberSectors);
			//funcion que devuelve un sector secuencial
			static void getSequentialSector(long long &selectSector,long long numberSectors);
			
			//abre el dispositivo
			void init(std::string &device);
			//lee la informacion de la extructura fisica del disco
			void readGeometryInfo();
			
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
			DeviceMeasuring(std::shared_ptr<DeviceInfo> &deviceInfo,DeviceMeasuring::mode mode,bool (*errorAsync)(DeviceMeasuring*,int,void*),void *paramError);
			//destructor
			~DeviceMeasuring();
			//optiene la medicion y reinia el contador
			void getMeasuringAndReset(DeviceMeasuring::measuring &measuring);
			//establece el modo de lectura
			void setMode(DeviceMeasuring::mode mode);
			//devualve el tamño del dispostivo
			long long getSize();
	};

#endif
