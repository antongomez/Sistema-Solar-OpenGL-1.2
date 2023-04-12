
#define MAX_OBJ 50 // Numero maximo de objetos
#define MAX_SAT 10 // Numero maximo de satelites de cada objeto

// Definimos el objeto generico

typedef struct {

								// Color del objeto en RGB
	GLfloat r;
	GLfloat g;
	GLfloat b;

	float distancia;			// Distancia al objeto que orbita
	float velocidad_trans;		// Velocidad de translacion
	float angulo_trans;			// Angulo de translacion
	float velocidad_rot;		// Velocidad de rotacion
	float angulo_rot;			// Angulo de rotacion
	int tamano;					// Tamano del objeto
	int listarender;			// Lista de renderizacion
	void* satelites[MAX_SAT];	// Array con sus satelites
	int num_sat;				// Numero de satelites

} objeto;

//#define W_WIDTH 650			// Ancho de la ventana
//#define W_HEIGHT 650		// Alto de la ventana

#define CR 0.0175
#define MYTIEMPO 41
#define TELE 0


#define INCREMENTO .01
#define DISTANCIA 3000
#define PI 3.141592

#define NEAR_VOYAGER 1000.0f
#define FAR_VOYAGER 6000.0f
#define NEAR_TELESCOPIO 20.0f
#define FAR_TELESCOPIO 2500.0f

void myCamara(int width, int height);

void myTeclado(unsigned char tras, int x, int y);
void myTeclasespeciales(int cursor, int x, int y);
void myTelescopio(float distancia, float angulo, float distancia_obj, float angulo_obj, int width, int height);
void myTelescopio2(float distancia, float angulo, float distancia_obj, float angulo_obj, int width, int height);
void myTelescopio3(float distancia, float angulo, float distancia_obj, float angulo_obj, int width, int height);
int orbitasActivadas();