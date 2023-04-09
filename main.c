#include <windows.h>	// Variables de windows
#include <glut.h>		// Incluimos GLUT
#include <GL/gl.h>		// Incluimos GL
#include <gl/glu.h>
#include <stdio.h>
#include <math.h>
#include "definiciones.h"
#include "esfera_12.h"


#define GL_PI 3.14f		// NO SE PARA QUE VALE ESTO

// Angulos de rotacion
static GLfloat xRot = 0.0f;		// ESTO YO CREO QUE TAMPOCO SIRVE PARA NADA
static GLfloat yRot = 0.0f;

int width = 650;
int height = 650;

// Para recordar como es la estructura:
/*
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
*/


// Aqui solo incluimos hasta listarender
objeto sol = { 1.0f, 1.0f, 0.0f, 0, 0, 0, 1, 0, 100, 0 };
objeto mercurio = { 0.7f, 0.7f, 0.7f, 200, 8, 0, 10, 0, 20, 0 };
objeto venus = { 0.9f, 0.5f, 0.0f, 350, 6, 0, 10, 0, 40, 0 };
objeto tierra = { 0.0f, 1.0f, 1.0f, 500, 5, 0, 15, 0, 40, 0 };
objeto luna = { 1.0f, 1.0f, 1.0f, 100, 2, 0, 0, 0, 10, 0 };
objeto marte = { 1.0f, 0.0f, 0.0f, 800, 2, 0, 10, 0, 30, 0 };
objeto jupiter = { 0.4f, 0.4f, 0.4f, 1000, 4, 0, 8, 0, 70, 0 };
objeto saturno = { 0.6f, 0.6f, 0.8f, 1200, 2, 0, 10, 0, 60, 0 };
objeto urano = { 1.0f, 0.0f, 0.0f, 1400, 2, 0, 10, 0, 50, 0 };
objeto neptuno = { 1.0f, 0.0f, 0.0f, 1500, 2, 0, 10, 0, 15, 0 };

camara = 1;

void dibujarObjeto(objeto* obj) {
	int i;

	if (orbitasActivadas()) {
		// Dibujamos la orbita
		glColor3f(1.0f, 1.0f, 1.0f);
		glPushMatrix();
		glRotatef(90.0, 1.0, 0.0, 0.0);
		glutWireTorus(0.5f, obj->distancia, 1, 100);
		glPopMatrix();
	}

	// Dibujamos el objeto
	glColor3f(obj->r, obj->g, obj->b);
	glPushMatrix();
	glRotatef(obj->angulo_trans, 0.0, 1.0, 0.0);
	glTranslatef(obj->distancia, 0.0, 0.0);
	glPushMatrix();
	glRotatef(obj->angulo_rot, 0, 1, 0);
	glScalef(obj->tamano, obj->tamano, obj->tamano);
	glCallList(obj->listarender);
	glPopMatrix();
	for (i = 0; i < obj->num_sat; i++) {
		dibujarObjeto((obj->satelites)[i]);
	}
	glPopMatrix();
}

// Funcion de dibujo
void myDisplay(void) {

	// Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	switch (camara) {
	case 1:
		myCamara(width, height);
		break;
	case 2:
		myTelescopio(tierra.distancia, tierra.angulo_trans, sol.distancia, sol.angulo_trans, width, height);
		break;
	case 3:
		myTelescopio(tierra.distancia, tierra.angulo_trans, mercurio.distancia, mercurio.angulo_trans, width, height);
		break;
	case 4:
		myTelescopio(tierra.distancia, tierra.angulo_trans, marte.distancia, marte.angulo_trans, width, height);
		break;
	case 5:
		myTelescopio2(tierra.distancia, tierra.angulo_trans, luna.distancia, luna.angulo_trans, width, height);
		break;
	case 6:
		myTelescopio3(tierra.distancia, tierra.angulo_trans, luna.distancia, luna.angulo_trans, width, height);
		break;
	}

	// Se activa la matriz del modelo
	glMatrixMode(GL_MODELVIEW);
	// Inicializamos la matriz del modelo a la identidad
	glLoadIdentity();

	// Establecemos el modo de rasterizacion
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Dibujamos el sol
	// El resto de objetos se iran dibujando con las llamadas
	// recursivas de la funcion dibujarObjeto
	dibujarObjeto(&sol);

	glFlush();
	glutSwapBuffers();
}

void idle(void) {
	// glutPostRedisplay();
}

void moverObjeto(objeto* obj) {
	obj->angulo_trans += obj->velocidad_trans;
	if (obj->angulo_trans > 360) obj->angulo_trans -= 360;
	obj->angulo_rot += obj->velocidad_rot;
	if (obj->angulo_rot > 360) obj->angulo_rot -= 360;
}


// LE TUVE QUE METER UN INT COMO ARGUMENTO, PORQUE SI NO DABA ERROR EL glutTimerFunc().
void myMovimiento(int i) {
	moverObjeto(&sol);
	moverObjeto(&mercurio);
	moverObjeto(&venus);
	moverObjeto(&tierra);
	moverObjeto(&luna);
	moverObjeto(&marte);
	moverObjeto(&jupiter);
	moverObjeto(&saturno);
	moverObjeto(&urano);
	moverObjeto(&neptuno);

	glutPostRedisplay();

	glutTimerFunc(MYTIEMPO, myMovimiento, 10);

}

void onMenu(int opcion) {
	switch (opcion) {
	case 1:
		camara = 1;
		break;
	case 2:
		camara = 2;
		break;
	case 3:
		camara = 3;
		break;
	case 4:
		camara = 4;
		break;
	case 5:
		camara = 5;
		break;
	case 6:
		camara = 6;
		break;
	}
	glutPostRedisplay();
}

void myMenu(void) {
	int menuFondo;			// Dijo que le cambiemos el nombre

	menuFondo = glutCreateMenu(onMenu);
	glutAddMenuEntry("Voyager", 1);
	glutAddMenuEntry("Sol desde Tierra", 2);
	glutAddMenuEntry("Mercurio desde Tierra", 3);
	glutAddMenuEntry("Marte desde Tierra", 4);
	glutAddMenuEntry("Luna desde Tierra", 5);
	glutAddMenuEntry("Tierra desde Luna", 6);


	glutAttachMenu(GLUT_RIGHT_BUTTON);
}



// Funcion de reescalado
void changeSizec(GLint newWidth, GLint newHeight) {
	glViewport(0, 0, newWidth, newHeight);
	width = newWidth;
	height = newHeight;
}




int main(int argc, char** argv) {

	int listarender = 0;

	glutInit(&argc, argv);
	//Posicion de la ventana
	glutInitWindowPosition(50, 50);
	//Tamano de la ventana
	glutInitWindowSize(width, height);
	//Inicializa el modo de display, RGBA y Doble buffer
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	//Crear la ventana
	glutCreateWindow("Sistema Solar");

	//Detectar profundidad de objetos y no dibujar caras ocultas
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//Normalizar las normales
	glEnable(GL_NORMALIZE);

	//Eventos

	//Funcion que mira el teclado
	glutKeyboardFunc(myTeclado);
	glutSpecialFunc(myTeclasespeciales);
	//Funcion de dibujo
	glutDisplayFunc(myDisplay);
	//Funcion de actualizacion
	glutIdleFunc(idle);
	//Funcion de reescalado
	glutReshapeFunc(changeSizec);


	//Color con el que se limpian los buffers
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	myMovimiento(1);
	myMenu();
	myCamara(width, height);

	// Establecemos el modelado de cada objeto (la mayoria son esferas)
	listarender = myEsfera();
	sol.listarender = listarender;
	mercurio.listarender = listarender;
	tierra.listarender = listarender;
	luna.listarender = listarender;
	marte.listarender = listarender;
	venus.listarender = listarender;
	jupiter.listarender = listarender;
	saturno.listarender = listarender;
	urano.listarender = listarender;
	neptuno.listarender = listarender;

	// Establecemos cuales son los objetos que orbitan a otros
	sol.satelites[0] = &mercurio;
	sol.satelites[1] = &venus;
	sol.satelites[2] = &tierra;
	sol.satelites[3] = &marte;
	sol.satelites[4] = &jupiter;
	sol.satelites[5] = &saturno;
	sol.satelites[6] = &urano;
	sol.satelites[7] = &neptuno;
	sol.num_sat = 8;
	tierra.satelites[0] = &luna;
	tierra.num_sat = 1;

	mercurio.num_sat = 0;
	venus.num_sat = 0;
	luna.num_sat = 0;
	marte.num_sat = 0;
	jupiter.num_sat = 0;
	saturno.num_sat = 0;
	urano.num_sat = 0;
	neptuno.num_sat = 0;


	//Empieza el bucle principal
	glutMainLoop();
	return 0;
}