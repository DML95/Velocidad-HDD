
//crea eventos de temporizacion enviados a una ventana de windows
class Temporizador{
	private:
		UINT_PTR hTemporizador;
	public:
		//Constructor
		Temporizador(HWND hVentana,int tiempo);
		
		//Destructor
		~Temporizador();
};
