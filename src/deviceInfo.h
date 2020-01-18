#ifndef DEVICEINFO_H
#define DEVICEINFO_H

	#include<string>
	#include<vector>

	#include "winapi.h"

	//optine informacion del dispositivo
	class DeviceInfo{
		private:
			const std::string device;
			std::vector<STORAGE_DEVICE_DESCRIPTOR> storageDeviceDescriptorBuffer;
			std::vector<VOLUME_DISK_EXTENTS> volumeDiskExtentsBuffer;

			//abre el dispositivo
			HANDLE init(std::string &device);
			//lee la descripcion del dispostivo
			void readStorageDeviceDescriptor(HANDLE handle);
			//lee la informacion de las particiones si la tiene
			void readPartitionsInfo(HANDLE handle);
		public:
			//constructor
			DeviceInfo(std::string &device);
			//getter y setter
			std::string get();
			//devuelve el id del producto
			std::string getProductId();
			//devuelve si la unidad es un disco fisico (no particion)
			bool isPhysical();
			//devuelve la los discos asignados a una particon
			const std::vector<VOLUME_DISK_EXTENTS>& getPartitionsInfo();
			//devuelve nombre dos del dispositivo si existe
			static std::string getDosDevice(const std::string &device);
	};

#endif
