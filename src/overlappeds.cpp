#include "overlappeds.h"

Overlappeds::Overlappeds():
		events(MAXIMUM_WAIT_OBJECTS),
		overlappeds(MAXIMUM_WAIT_OBJECTS){
	for(int cont=0;cont<MAXIMUM_WAIT_OBJECTS;++cont){
		const HANDLE event=CreateEvent(NULL,true,true,NULL);
		events[cont]=event;
		OVERLAPPED *overlapped=&overlappeds[cont];
		overlapped->hEvent=event;
		overlapped->Pointer=0;
	}
}

Overlappeds::~Overlappeds(){
	for(int cont=0;cont<MAXIMUM_WAIT_OBJECTS;++cont){
		CloseHandle(events[cont]);
	}
}

OVERLAPPED* Overlappeds::wait(const bool waitAll){
	OVERLAPPED* overlapped=0;
	const int id=WaitForMultipleObjects(events.size(),events.data(),waitAll,INFINITE)-WAIT_OBJECT_0;
	if(id>=0){
		overlapped=&overlappeds[id];
	}
	return overlapped;
}
