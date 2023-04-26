#include <windows.h>	

#include <glut.h>		
#include <gl/gl.h>
#include <gl/glu.h>

#define MAX_OBJ 50 // Numero maximo de objetos
#define MAX_SAT 10 // Numero maximo de satelites de cada objeto

// Definimos el objeto generico

typedef struct {

	// Color del objeto en RGB
	GLfloat r;
	GLfloat g;
	GLfloat b;

	float distancia;						// Distancia al objeto que orbita
	float velocidad_trans;					// Velocidad de translacion
	float angulo_trans;						// Angulo de translacion
	float velocidad_rot;					// Velocidad de rotacion
	float angulo_rot;						// Angulo de rotacion
	int tamano;								// Tamano del objeto
	int listarender;						// Lista de renderizacion
	void* satelites[MAX_SAT];				// Array con sus satelites
	int num_sat;							// Numero de satelites
	void (*luz_obxecto)();					// Funcion que activa a luz que afecta ao obxecto
	void (*luz_obxecto_desactivar)();		// Funcion que desactiva a luz que afecta ao obxecto
	GLuint textura;							// Identificador da textura
	GLuint texturaAneis;					// Indentificador da textura dos aneis en caso de telos
	int listarenderAneis;					// Lista de renderizacion para os aneis se os ten
} objeto;

#define CR 0.0175
#define MYTIEMPO 41
#define TELE 0


#define INCREMENTO_XIRAR .02
#define INCREMENTO_ACERCAR 30
#define DISTANCIA 2500
#define PI 3.141592

#define DISTANCIA_FONDO 6000.0f

/* O fondo debe poder verse e esta a 6000 unidades do centro.
* A distancia máxima da camara con respecto ao centro e 4000
* co que o plano FAR debe estar, a mais de 10000.*/
#define FAR_VOYAGER 10000.0f
/* Para poder acercar ponhemos un near baixo */
#define NEAR_VOYAGER 50.0f
#define NEAR_TELESCOPIO 20.0f
#define FAR_TELESCOPIO 7000.0f

#define INDICE_LISTA_ESFERA 1
#define INDICE_LISTA_CUBO 2
#define INDICE_LISTA_TOURO 3

void myCamara(int width, int height);

void myTeclado(unsigned char tras, int x, int y);
void myTeclasespeciales(int cursor, int x, int y);
void myTelescopio(float distancia, float angulo, float distancia_obj, float angulo_obj, int width, int height);
void myTelescopio2(float distancia, float angulo, float distancia_obj, float angulo_obj, int width, int height);
void myTelescopio3(float distancia, float angulo, float distancia_obj, float angulo_obj, int width, int height);
int orbitasActivadas();

int crearListaEsfera();
int crearListaCubo(int textura);

int orbitasActivadas();
int lucesActivadas();
int solEncendido();
int sistemaParado();