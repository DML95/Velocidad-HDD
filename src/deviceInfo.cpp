#include <securitybaseapi.h>
#include <iostream>
#include <cstring>

#include "deviceInfo.h"

HANDLE DeviceInfo::init(std::string &device){
	HANDLE handle=CreateFile(
			device.c_str(),
			0,
			FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,
			0,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL|FILE_FLAG_NO_BUFFERING|FILE_FLAG_OVERLAPPED,
			0);
	if(handle==INVALID_HANDLE_VALUE){
		throw (int)GetLastError();
	}
	return handle;
}

void DeviceInfo::readStorageDeviceDescriptor(HANDLE handle){
	//iniciamos storagePropertyQuery
	STORAGE_PROPERTY_QUERY storagePropertyQuery;
	std::memset(&storagePropertyQuery,0,sizeof(STORAGE_PROPERTY_QUERY));
    storagePropertyQuery.PropertyId=StorageDeviceProperty;
    storagePropertyQuery.QueryType=PropertyStandardQuery;
    //optenemos el tamaño para el buffer
    STORAGE_DESCRIPTOR_HEADER storageDescriptorHeader;
    unsigned long bytesReturned=0;
    if(DeviceIoControl(handle,IOCTL_STORAGE_QUERY_PROPERTY,
    		&storagePropertyQuery,sizeof(STORAGE_PROPERTY_QUERY),
        	&storageDescriptorHeader,sizeof(STORAGE_DESCRIPTOR_HEADER),&bytesReturned,NULL)){
        int size=storageDescriptorHeader.Size;
        this->storageDeviceDescriptorBuffer.resize(size);
        //rellenamos el buffer
        if(DeviceIoControl(handle,IOCTL_STORAGE_QUERY_PROPERTY,
		        &storagePropertyQuery,sizeof(STORAGE_PROPERTY_QUERY),
		        this->storageDeviceDescriptorBuffer.data(),size,&bytesReturned,NULL)){
		}else this->storageDeviceDescriptorBuffer.clear();
	}
}

void DeviceInfo::readPartitionsInfo(HANDLE handle){
	VOLUME_DISK_EXTENTS volumeDiskExtents;
    unsigned long bytesReturned=0;
    if(DeviceIoControl(handle,IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS,
    		NULL,0,
        	&volumeDiskExtents,sizeof(VOLUME_DISK_EXTENTS),&bytesReturned,NULL)){
        if(volumeDiskExtents.NumberOfDiskExtents){
        	int size=sizeof(VOLUME_DISK_EXTENTS)+sizeof(DISK_EXTENT)*(volumeDiskExtents.NumberOfDiskExtents-1);
	        this->volumeDiskExtentsBuffer.resize(size);
	        if(!DeviceIoControl(handle,IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS,
    		NULL,0,
        	this->volumeDiskExtentsBuffer.data(),size,&bytesReturned,NULL)){
        		this->volumeDiskExtentsBuffer.clear();
			}
	    }
	}
}

DeviceInfo::DeviceInfo(std::string device):
		device(device),
		storageDeviceDescriptorBuffer(0),
		volumeDiskExtentsBuffer(0){
	HANDLE handle=this->init(device);
	this->readStorageDeviceDescriptor(handle);
	this->readPartitionsInfo(handle);
	CloseHandle(handle);
	std::clog<<"[DeviceInfo] iniciando unidad\n\tnombre: "<<device<<
			"\n\tid producto: "<<this->getProductId()<<
			"\n\tdispostivo fisico: "<<this->isPhysical()<<std::endl;
}

std::string DeviceInfo::get(){
	return device;
}

std::string DeviceInfo::getProductId(){
	std::string productId;
	if(!this->storageDeviceDescriptorBuffer.empty()){
		STORAGE_DEVICE_DESCRIPTOR *storageDeviceDescriptor=this->storageDeviceDescriptorBuffer.data();
		productId=std::string((char*)storageDeviceDescriptor+storageDeviceDescriptor->ProductIdOffset);
	}
	return productId;
}

bool DeviceInfo::isPhysical(){
	return this->volumeDiskExtentsBuffer.empty();
}

const std::vector<VOLUME_DISK_EXTENTS>& DeviceInfo::getPartitionsInfo(){
	return this->volumeDiskExtentsBuffer;
}

std::string DeviceInfo::getDosDevice(const std::string &device){
	char dosDevice[127];
	int size=QueryDosDevice(device.c_str(),dosDevice,sizeof(dosDevice));
	return std::string(dosDevice,size);
}
