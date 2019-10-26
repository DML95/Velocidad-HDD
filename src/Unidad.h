#ifndef UNIDAD_H
#define UNIDAD_H
	
	//abre una unidad y devuelve y lee sus datos de manera asincrona
	class Unidad{
		private:
			HANDLE hUnidad;
			int byteSector;
			long long numSector;
		public:
			
			//Constructor
			Unidad(char unidad);
			
			//Destructor
			~Unidad();
			
			//getter Setter
			HANDLE get();
			
			int getByteSector();
			
			long long getNumSector();
			
			bool getRegistro(const char *datos,OVERLAPPED *overlapped);
			
			void cancelarOperacion();
	};

#endif
