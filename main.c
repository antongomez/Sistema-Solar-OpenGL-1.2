#include <windows.h>	// Variables de windows

#include <glut.h>		// Incluimos GLUT
#include <gl/gl.h>
#include <gl/glu.h>
#include <string.h>

#include <stdio.h>
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "definiciones.h"

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
	int textura								// Identificador da textura
	int listarenderAneis					// Lista de renderizacion para os aneis se os ten
*/

// Aqui solo incluimos hasta listarender
objeto sol = { 1.0f, 1.0f, 1.0f, 0, 0, 0, 1, 0, 100, 0 };
objeto mercurio = { 1.0f, 1.0f, 1.0f, 200, 5, 0, 6, 0, 20, 0 };
objeto venus = { 1.0f, 1.0f, 1.0f, 350, 4.5, 0, 6, 0, 40, 0 };
objeto tierra = { 1.0f, 1.0f, 1.0f, 500, 4, 0, 5, 0, 40, 0 };
objeto luna = { 1.0f, 1.0f, 1.0f, 100, 3, 0, 0, 0, 10, 0 };
objeto iss = { 1.0f, 1.0f, 1.0f, 60, 4, 0, 0, 0, 3, 0 };
objeto marte = { 1.0f, 1.0f, 1.0f, 700, 2.5, 0, 4, 0, 30, 0 };
objeto jupiter = { 1.0f, 1.0f, 1.0f, 900, 2, 0, 3, 0, 70, 0 };
objeto saturno = { 1.0f, 1.0f, 1.0f, 1100, 1.5, 0, 3, 0, 60, 0 };
objeto urano = { 1.0f, 1.0f, 1.0f, 1300, 1, 0, 3, 0, 50, 0 };
objeto neptuno = { 1.0f, 1.0f, 1.0f, 1500, 0.5, 0, 3, 0, 50, 0 };

// Variables de iluminacion para o sol
GLfloat ambiente_sol[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat difusa_sol[] = { 0, 0, 0, 1.0f };
GLfloat especular_sol[] = { 0, 0, 0, 1.0f };

// Variables de iluminacion para os planetas
GLfloat ambiente[] = { 0, 0, 0, 1.0f };
GLfloat difusa[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat especularRef[] = { 0.8f, 0.8f, 0.8f, 0.8f };
GLfloat especular[] = { 0.5f, 0.5f, 0.5f, 1.0f };

// Variable para definir a posicion do foco
GLfloat posLuz[] = { 0.0f, 0.0f, 0.0f, 1.0f };
// Define a direccion da luz, ainda que 
// non a imos usar porque imos facer que 
// a apertura do foco sexa de 180 graos
GLfloat dirFoco[] = { 1.0f, 0.0f, 1.0f };

// Funcions para activar e desactivar as orbitas
void luz_ambiente();
void luz_ambiente_desactivar();

// Funcions para debuxar un touro texturizado
void arrayTouro(double r, double R, int rSeg, int cSeg, int textura);
int crearListaTouro(int textura);

// Variable para a textura do fondo
GLuint textura_milky;

// Variable para almacenar o indice da lista que debuxara o fondo
int listarenderFondo = 0;

// Camara inicial (Voyager)
camara = 1;

void dibujarObjeto(objeto* obj) {

	// Aplicamos as luces so se estan activadas
	if (!lucesActivadas()) {
		glDisable(GL_LIGHTING);
	}
	else {
		glEnable(GL_LIGHTING);
	}

	// Debuxamos as orbitas so se estan activadas
	if (orbitasActivadas() && (obj->distancia != 0)) {
		// Desactivamos as texturas
		glDisable(GL_TEXTURE_2D);

		// Activamos a luz das orbitas
		luz_ambiente();

		// Debuxamos la orbita
		glColor3f(0.5f, 0.5f, 0.5f);

		glPushMatrix();
		glRotatef(90.0, 1.0, 0.0, 0.0);
		glScalef(1.0, 1.0, 0.1);

		glutWireTorus(0.5f, obj->distancia, 10, 100);

		glPopMatrix();

		luz_ambiente_desactivar();

		// Volvemos a activar as texturas
		glEnable(GL_TEXTURE_2D);
	}

	// Activamos a iluminacion do obxecto
	obj->luz_obxecto();

	// Debuxamos o obxecto
	glColor3f(obj->r, obj->g, obj->b);

	glPushMatrix();

	glRotatef(obj->angulo_trans, 0.0, 1.0, 0.0);
	glTranslatef(obj->distancia, 0.0, 0.0);

	glPushMatrix();

	glRotatef(obj->angulo_rot, 0, 1, 0);
	glScalef(obj->tamano, obj->tamano, obj->tamano);

	// Habilitamos a textura
	glBindTexture(GL_TEXTURE_2D, obj->textura);

	// Renderizamos o obxecto
	glCallList(obj->listarender);

	glPopMatrix();

	if (obj->listarenderAneis) {
		glColor3f(1.0f, 1.0f, 1.0f);
		glPushMatrix();
		// Facemos que o touro rote co planeta
		glRotatef(obj->angulo_rot, 0, 1, 0);
		// Ponhemos o touro en horizontal
		glRotatef(90.0, 1.0, 0.0, 0.0);
		glScalef(1.0f, 1.0f, 0.1f);
		glCallList(obj->listarenderAneis);
		glPopMatrix();
	}

	// Desactivamos a luz do obxecto
	obj->luz_obxecto_desactivar();

	// Debuxamos os satelites de cada planeta ou estrela
	for (int i = 0; i < obj->num_sat; i++) {
		dibujarObjeto((obj->satelites)[i]);
	}

	glPopMatrix();
}

// Funcion para debuxar o fondo e asignarlle unha textura
void debuxarFondo() {

	glColor3f(1.0f, 1.0f, 1.0f);

	// Activamos a luz ambiente para o fondo
	luz_ambiente();

	// Para que se poidan ver tamen as caras interiores da esfera
	glDisable(GL_CULL_FACE);

	// Escalamos a esfera que debuxa o fondo
	glPushMatrix();
	glRotatef(180.0, 0.0, 1.0, 0.0);
	glScalef(DISTANCIA_FONDO, DISTANCIA_FONDO, DISTANCIA_FONDO);

	// Asignamoslle a textura ao fondo
	glBindTexture(GL_TEXTURE_2D, textura_milky);

	// Renderizamos o fondo que sera unha esfera
	glCallList(listarenderFondo);

	glPopMatrix();

	glEnable(GL_CULL_FACE);

	luz_ambiente_desactivar();
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

	// Debuxamos o fondo
	debuxarFondo();

	// Debuxamos o sol
	// El resto de objetos se iran dibujando con las llamadas
	// recursivas de la funcion dibujarObjeto
	dibujarObjeto(&sol);

	glFlush();
	glutSwapBuffers();
}

void idle(void) {}

void moverObjeto(objeto* obj) {
	obj->angulo_trans += obj->velocidad_trans;
	if (obj->angulo_trans > 360) obj->angulo_trans -= 360;
	obj->angulo_rot += obj->velocidad_rot;
	if (obj->angulo_rot > 360) obj->angulo_rot -= 360;
}

void myMovimiento(int i) {
	if (!sistemaParado()) {
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
	}

	glutTimerFunc(MYTIEMPO, myMovimiento, 10);

}

void onMenu(int opcion) {
	camara = opcion;
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
	if (solEncendido()) {
		glEnable(GL_LIGHT0);
	}
}

// Funcion para desactivar a luz que ilumina as orbitas e o sol
void luz_ambiente_desactivar() {
	if (solEncendido()) {
		glDisable(GL_LIGHT0);
	}
}

// Funcion para activar a luz dos planetas
void luz_planeta() {
	if (solEncendido()) {
		// Habilitamos a luz dos planetas
		glEnable(GL_LIGHT1);

		// Definimos as propiedades de brillo metalico
		glMaterialfv(GL_FRONT, GL_SPECULAR, especularRef);
		// Podemos establecer o brillo entre [0,128]
		glMateriali(GL_FRONT, GL_SHININESS, BRILLO_PLANETAS);
	}
}

// Funcion para activar a luz da iss
void luz_iss() {
	if (solEncendido()) {
		// Habilitamos a luz dos planetas
		glEnable(GL_LIGHT1);

		// Definimos as propiedades de brillo metalico
		glMaterialfv(GL_FRONT, GL_SPECULAR, especularRef);
		// Podemos establecer o brillo entre [0,128]
		glMateriali(GL_FRONT, GL_SHININESS, BRILLO_ISS);
	}
}

// Funcion para desactivar a luz dos planetas e da iss
void luz_planeta_desactivar() {
	glDisable(GL_LIGHT1);
}


// Funcion para cargar as texturas
void cargar_textura(char* ruta_arquivo, GLuint* textura) {
	char* destino = "texturas/";
	char* ruta = NULL;
	// Tamanho que ocupara o nome do arquivo
	int tamanho_cadea = (strlen(ruta_arquivo) + strlen(destino) + 1) * sizeof(char);

	ruta = (char*)malloc(tamanho_cadea);

	if (ruta != NULL) {
		strcpy_s(ruta, tamanho_cadea, destino);
		strcat_s(ruta, tamanho_cadea, ruta_arquivo);
	}
	else {
		printf("Erro de memoria, non se pode reservar memoria para o nome do ficheiro %s\n", ruta_arquivo);
		exit(EXIT_FAILURE);
	}

	glGenTextures(1, textura);
	// todas as futuras operacions sobre GL_TEXTURE_2D agora tenhen efecto sobre esta textura
	glBindTexture(GL_TEXTURE_2D, *textura);

	// Establecemos os parametros cando a textura non colle
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Establecemos os parametros para filtrar a textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Antes de cargar a imaxe invertimola para que quede ao dereito
	stbi_set_flip_vertically_on_load(1);

	// Cargar a imaxe, creamos a textura e xeramos os mipmaps
	int ancho, alto, nrChannels;
	unsigned char* data = stbi_load(ruta, &ancho, &alto, &nrChannels, 0);

	if (data != NULL) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ancho, alto, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	else {
		printf("Erro cargando a textura");
	}
	stbi_image_free(data);
	free(ruta);
}

void inicializarObxectos() {

	// Establecemos el modelado de cada objeto (la mayoria son esferas)
	int listarender = crearListaEsfera();
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

	// O fondo tamen e unha esfera
	listarenderFondo = listarender;

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
	tierra.satelites[0] = &iss;
	tierra.satelites[1] = &luna;
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
	iss.luz_obxecto = luz_iss;
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

	cargar_textura("2k_sun.jpg", &sol.textura);
	cargar_textura("2k_mercury.jpg", &mercurio.textura);
	cargar_textura("2k_venus_atmosphere.jpg", &venus.textura);
	cargar_textura("2k_earth_daymap.jpg", &tierra.textura);
	cargar_textura("2k_moon.jpg", &luna.textura);
	cargar_textura("estacion.jpg", &iss.textura);
	cargar_textura("2k_mars.jpg", &marte.textura);
	cargar_textura("2k_jupiter.jpg", &jupiter.textura);
	cargar_textura("2k_saturn.jpg", &saturno.textura);
	cargar_textura("2k_uranus.jpg", &urano.textura);
	cargar_textura("2k_neptune.jpg", &neptuno.textura);
	cargar_textura("2k_stars_milky_way.jpg", &textura_milky);

	sol.listarenderAneis = 0;
	mercurio.listarenderAneis = 0;
	tierra.listarenderAneis = 0;
	iss.listarenderAneis = 0;
	luna.listarenderAneis = 0;
	marte.listarenderAneis = 0;
	venus.listarenderAneis = 0;
	jupiter.listarenderAneis = 0;
	int texturaAneisTouro = 0;
	cargar_textura("textura_aneis.jpg", &texturaAneisTouro, GL_RGB);
	saturno.listarenderAneis = crearListaTouro(texturaAneisTouro);
	urano.listarenderAneis = 0;
	neptuno.listarenderAneis = 0;
}

void inicializar_iluminacion() {
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

	// Definimos o seguimento da cor como propiedade luminosa nos materiais
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}


int main(int argc, char** argv) {

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
	//Color con el que se limpian los buffers
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	// Deshabilitar la vista de las caras ocultas
	glEnable(GL_CULL_FACE);
	//Normalizar las normales
	glEnable(GL_NORMALIZE);
	// Habilitamos as texturas
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);

	inicializar_iluminacion();

	//Funciones que mira el teclado
	glutKeyboardFunc(myTeclado);
	glutSpecialFunc(myTeclasespeciales);
	//Funcion de dibujo
	glutDisplayFunc(myDisplay);
	//Funcion de actualizacion
	glutIdleFunc(idle);
	//Funcion de reescalado
	glutReshapeFunc(changeSizec);

	myMovimiento(1);
	myMenu();
	myCamara(width, height);

	inicializarObxectos();

	//Empieza el bucle principal
	glutMainLoop();
	return EXIT_SUCCESS;
}

void arrayTouro(double r, double R, int rSeg, int cSeg, int textura) {
	glFrontFace(GL_CW);

	glBindTexture(GL_TEXTURE_2D, textura);
	//modo de fusion de la textura
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	for (int i = 0; i < rSeg; i++) {
		glBegin(GL_QUAD_STRIP);
		for (int j = 0; j <= cSeg; j++) {
			for (int k = 0; k <= 1; k++) {
				//pasos
				double s = (i + k) % rSeg + 0.5;
				double t = j % (cSeg + 1);
				//coordenadas del toro
				double x = (R + r * cos(s * 2 * PI / rSeg)) * cos(t * 2 * PI / cSeg);
				double y = (R + r * cos(s * 2 * PI / rSeg)) * sin(t * 2 * PI / cSeg);
				double z = r * sin(s * 2 * PI / rSeg);
				//coordenadas de textura
				double u = (i + k) / (float)rSeg;
				double v = t / (float)cSeg;

				glTexCoord2d(u, v);
				glNormal3f(2 * x, 2 * y, 2 * z);
				glVertex3d(2 * x, 2 * y, 2 * z);
			}
		}
		glEnd();
	}
	glFrontFace(GL_CCW);
}

int crearListaTouro(int textura) {
	// Creamos unha lista cos aneis do touro
	int indice = glGenLists(1);
	glNewList(indice, GL_COMPILE);
	arrayTouro(12, 60, 8, 32, textura);
	glEndList();

	return indice;
}