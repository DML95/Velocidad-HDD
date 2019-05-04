
//abre una unidad y devuelve y lee sus datos
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
		int getByteSector();
		
		long long getNumSector();
		
		bool getRegistro(char *datos,long long numSector);
		
		void cancelarOperacion();
};
