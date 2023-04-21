#include <windows.h>	// Variables de windows

#include <glut.h>		// Incluimos GLUT
#include <GL/gl.h>		// Incluimos GL
#include <gl/glu.h>

#include <stdio.h>

#include <math.h>

#include "definiciones.h"
#include "esfera_12.h"

// Variables que determinan o ancho da venta
int width = 650;
int height = 650;

// Para recordar como es la estructura:
/*
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
	void (*luz_obxecto)(void);				// Funcion que activa a luz que afecta ao obxecto
	void (*luz_obxecto_desactivar)(void);	// Funcion que desactiva a luz que afecta ao obxecto
*/


// Aqui solo incluimos hasta listarender
objeto sol = { 1.0f, 1.0f, 0.0f, 0, 0, 0, 1, 0, 100, 0 };
objeto mercurio = { 0.7f, 0.7f, 0.7f, 200, 5, 0, 6, 0, 20, 0 };
objeto venus = { 0.9f, 0.5f, 0.0f, 350, 4.5, 0, 6, 0, 40, 0 };
objeto tierra = { 0.0f, 1.0f, 0.75f, 500, 4, 0, 5, 0, 40, 0 };
objeto luna = { 1.0f, 1.0f, 1.0f, 100, 3, 0, 0, 0, 10, 0 };
objeto iss = { 1.0f, 1.0f, 1.0f, 60, 4, 0, 0, 0, 3, 0 };
objeto marte = { 1.0f, 0.0f, 0.0f, 700, 2.5, 0, 4, 0, 30, 0 };
objeto jupiter = { 0.5f, 0.2f, 0.0f, 900, 2, 0, 3, 0, 70, 0 };
objeto saturno = { 1.0f, 0.7f, 0.0f, 1100, 1.5, 0, 3, 0, 60, 0 };
objeto urano = { 0.0f, 1.0f, 1.0f, 1300, 1, 0, 3, 0, 50, 0 };
objeto neptuno = { 0.0f, 0.0f, 1.0f, 1500, 0.5, 0, 3, 0, 50, 0 };

// Variables de iluminacion para o sol
GLfloat ambiente_sol[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat difusa_sol[] = { 0, 0, 0, 1.0f };
GLfloat especular_sol[] = { 0, 0, 0, 1.0f };

// Variables de iluminacion para os planetas
GLfloat ambiente[] = { 0, 0, 0, 1.0f };
GLfloat difusa[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat especularRef[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat especular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

// Variable para definir a posicion do foco
GLfloat posLuz[] = { 0.0f, 0.0f, 0.0f, 1.0f };
// Define a direccion da luz, ainda que 
// non a imos usar porque imos facer que 
// a apertura do foco sexa de 180 graos
GLfloat dirFoco[] = { 1.0f, 1.0f, 1.0f };

// Funcions para activar e desactivar as orbitas
void luz_ambiente();
void luz_ambiente_desactivar();


// Camara inicial (Voyager)
camara = 1;

void dibujarObjeto(objeto* obj) {
	int i;

	if (orbitasActivadas() && (obj->distancia != 0)) {
		// Activamos a luz das orbitas
		luz_ambiente();
		// Dibujamos la orbita
		glColor3f(1.0f, 1.0f, 1.0f);
		glPushMatrix();
		glRotatef(90.0, 1.0, 0.0, 0.0);
		glScalef(1.0, 1.0, 0.1);
		glutWireTorus(0.5f, obj->distancia, 10, 100);
		glPopMatrix();
		luz_ambiente_desactivar();
	}

	// Activamos a iluminacion do obxecto
	obj->luz_obxecto();


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
	if (obj->distancia == saturno.distancia) {
		// Anillos (solo Saturno)
		glPushMatrix();
		glRotatef(45.0, 0.0, 0.0, 1.0);
		glRotatef(90.0, 1.0, 0.0, 0.0);
		glutWireTorus(0.5f, 90, 1, 100);
		glutWireTorus(0.5f, 95, 1, 100);
		glutWireTorus(0.5f, 100, 1, 100);
		glutWireTorus(0.5f, 105, 1, 100);
		glutWireTorus(0.5f, 110, 1, 100);
		glutWireTorus(0.5f, 115, 1, 100);
		glPopMatrix();

	}

	// Desactivamos a luz do obxecto
	obj->luz_obxecto_desactivar();

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
		myTelescopio(tierra.distancia, tierra.angulo_trans, jupiter.distancia, jupiter.angulo_trans, width, height);
		break;
	case 6:
		myTelescopio(tierra.distancia, tierra.angulo_trans, saturno.distancia, saturno.angulo_trans, width, height);
		break;
	case 7:
		myTelescopio(tierra.distancia, tierra.angulo_trans, urano.distancia, urano.angulo_trans, width, height);
		break;
	case 8:
		myTelescopio(tierra.distancia, tierra.angulo_trans, neptuno.distancia, neptuno.angulo_trans, width, height);
		break;
	case 9:
		myTelescopio2(tierra.distancia, tierra.angulo_trans, luna.distancia, luna.angulo_trans, width, height);
		break;
	case 10:
		myTelescopio2(tierra.distancia, tierra.angulo_trans, iss.distancia, iss.angulo_trans, width, height);
		break;
	case 11:
		myTelescopio3(tierra.distancia, tierra.angulo_trans, luna.distancia, luna.angulo_trans, width, height);
		break;
	case 12:
		myTelescopio3(tierra.distancia, tierra.angulo_trans, iss.distancia, iss.angulo_trans, width, height);
		break;
	}

	// Se activa la matriz del modelo
	glMatrixMode(GL_MODELVIEW);
	// Inicializamos la matriz del modelo a la identidad
	glLoadIdentity();

	// Establecemos el modo de rasterizacion
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

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


void myMovimiento(int i) {
	moverObjeto(&sol);
	moverObjeto(&mercurio);
	moverObjeto(&venus);
	moverObjeto(&tierra);
	moverObjeto(&luna);
	moverObjeto(&iss);
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
	case 7:
		camara = 7;
		break;
	case 8:
		camara = 8;
		break;
	case 9:
		camara = 9;
		break;
	case 10:
		camara = 10;
		break;
	case 11:
		camara = 11;
		break;
	case 12:
		camara = 12;
		break;
	}
	glutPostRedisplay();
}

void myMenu(void) {
	int menuFondo;

	menuFondo = glutCreateMenu(onMenu);
	glutAddMenuEntry("Voyager", 1);
	glutAddMenuEntry("Sol desde Tierra", 2);
	glutAddMenuEntry("Mercurio desde Tierra", 3);
	glutAddMenuEntry("Marte desde Tierra", 4);
	glutAddMenuEntry("Jupiter desde Tierra", 5);
	glutAddMenuEntry("Saturno desde Tierra", 6);
	glutAddMenuEntry("Urano desde Tierra", 7);
	glutAddMenuEntry("Neptuno desde Tierra", 8);
	glutAddMenuEntry("Luna desde Tierra", 9);
	glutAddMenuEntry("ISS desde Tierra", 10);
	glutAddMenuEntry("Tierra desde Luna", 11);
	glutAddMenuEntry("Tierra desde ISS", 12);


	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// Funcion de reescalado
void changeSizec(GLint newWidth, GLint newHeight) {
	glViewport(0, 0, newWidth, newHeight);
	width = newWidth;
	height = newHeight;
}

// Funcion para activar a luz que ilumina as orbitas e o sol
void luz_ambiente() {
	glEnable(GL_LIGHT0);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT);
}

// Funcion para desactivar a luz que ilumina as orbitas e o sol
void luz_ambiente_desactivar() {
	glDisable(GL_LIGHT0);
}

// Funcion para activar a luz dos planetas
void luz_planeta() {
	// Habilitamos a luz dos planetas
	glEnable(GL_LIGHT1);

	// Definimos o seguimento da cor como propiedade luminosa
	// nos materiais
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Definimos as propiedades de brillo metalico
	glMaterialfv(GL_FRONT, GL_SPECULAR, especularRef);
	glMateriali(GL_FRONT, GL_SHININESS, 2);
}

// Funcion para desactivar a luz dos planetas
void luz_planeta_desactivar() {
	glDisable(GL_LIGHT1);
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
	// Deshabilitar la vista de las caras ocultas
	glEnable(GL_CULL_FACE);
	//Normalizar las normales
	glEnable(GL_NORMALIZE);

	// Definimos o modelo de iluminacion para o sol
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambiente_sol);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, difusa_sol);
	glLightfv(GL_LIGHT0, GL_SPECULAR, especular_sol);

	// Definimos o modelo de iluminacion para os planetas
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambiente);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, difusa);
	glLightfv(GL_LIGHT1, GL_SPECULAR, especular);
	glLightfv(GL_LIGHT1, GL_POSITION, posLuz);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, dirFoco);

	// Definimos un foco con aprtura de 180 graos
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 180.0f);

	// Habilitamos as luces no sistema solar
	glEnable(GL_LIGHTING);

	//Eventos

	//Funciones que mira el teclado
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
	iss.listarender = listarender;
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
	tierra.satelites[1] = &iss;
	tierra.num_sat = 2;

	mercurio.num_sat = 0;
	venus.num_sat = 0;
	luna.num_sat = 0;
	iss.num_sat = 0;
	marte.num_sat = 0;
	jupiter.num_sat = 0;
	saturno.num_sat = 0;
	urano.num_sat = 0;
	neptuno.num_sat = 0;

	// Establecemos as funcions que determinan o
	// modelo de iluminacion para cada planeta e 
	// o desactiven
	sol.luz_obxecto = luz_ambiente;
	mercurio.luz_obxecto = luz_planeta;
	venus.luz_obxecto = luz_planeta;
	tierra.luz_obxecto = luz_planeta;
	luna.luz_obxecto = luz_planeta;
	iss.luz_obxecto = luz_planeta;
	marte.luz_obxecto = luz_planeta;
	jupiter.luz_obxecto = luz_planeta;
	saturno.luz_obxecto = luz_planeta;
	urano.luz_obxecto = luz_planeta;
	neptuno.luz_obxecto = luz_planeta;

	sol.luz_obxecto_desactivar = luz_ambiente_desactivar;
	mercurio.luz_obxecto_desactivar = luz_planeta_desactivar;
	venus.luz_obxecto_desactivar = luz_planeta_desactivar;
	tierra.luz_obxecto_desactivar = luz_planeta_desactivar;
	luna.luz_obxecto_desactivar = luz_planeta_desactivar;
	iss.luz_obxecto_desactivar = luz_planeta_desactivar;
	marte.luz_obxecto_desactivar = luz_planeta_desactivar;
	jupiter.luz_obxecto_desactivar = luz_planeta_desactivar;
	saturno.luz_obxecto_desactivar = luz_planeta_desactivar;
	urano.luz_obxecto_desactivar = luz_planeta_desactivar;
	neptuno.luz_obxecto_desactivar = luz_planeta_desactivar;

	//Empieza el bucle principal
	glutMainLoop();
	return 0;
}