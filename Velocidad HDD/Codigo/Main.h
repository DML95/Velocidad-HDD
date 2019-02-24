
//inicio del programa
static const char *nombreClase="VelocidadHDD";

static Ventana *pVentana;
static Tabla *pTabla;
static Dispositivos *pDispositivos;
static Temporizador *pTemporizador;

static void error(char *error,int num);

//Arranca el bucle de mensajes
static int bucleMensajes(HWND hVentana);
//Funcion de recepcion de Eventos
static WNDPROC evento(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int main(int argc, char** argv);
