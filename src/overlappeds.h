#ifndef OVERLAPPEDS_H
#define OVERLAPPEDS_H

	#include<vector>

    #include "winapi.h"

	class Overlappeds{
		private:
			std::vector<HANDLE> events;
			std::vector<OVERLAPPED> overlappeds;
		public:
			//constructor
			Overlappeds();
			//destructor
			~Overlappeds();
			//espera hasta que ocurra un evento en uno o todos los overlappeds
			OVERLAPPED* wait(bool waitAll);
	};

#endif
