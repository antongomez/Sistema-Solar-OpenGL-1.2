#include <windows.h>		//Inclusion de variables de windows
#include <glut.h>			//Inclusion de GLUT
#include <GL/gl.h>			//Inclusion de GL		 	
#include <GL/glu.h>	
#include <stdio.h>
#include <math.h>			//Inclusion de librerias auxiliares	
#include "definiciones.h"

float alpha = 0;
float beta = PI / 18.0f;

float orbitas = 1;

float distancia = DISTANCIA;



void myCamara(int width, int height) {

	//Configuración de la matriz de proyeccion
	glMatrixMode(GL_PROJECTION);
	//La ponemos a uno
	glLoadIdentity();

	// Si el plano near lo situamos más lejos, no se ve neptuno si situamos la camara en el plano de las orbitas
	gluPerspective(45.0, (GLfloat)width / (GLfloat)height, NEAR_VOYAGER, FAR_VOYAGER);
	gluLookAt(((float)distancia * (float)sin(alpha) * cos(beta)), ((float)distancia * (float)sin(beta)), ((float)distancia * cos(alpha) * cos(beta)), 0, 0, 0, 0, cos(beta), 0);

}

void myTelescopio(float distancia, float angulo, float distancia_obj, float angulo_obj, int width, int height) {

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (GLfloat)width / (GLfloat)height, NEAR_TELESCOPIO, FAR_TELESCOPIO);
	gluLookAt(distancia * cos(angulo * PI / 180.0), 0, -1 * distancia * sin(angulo * PI / 180.0),
		distancia_obj * cos(angulo_obj * PI / 180.0), 0, -1 * distancia_obj * sin(angulo_obj * PI / 180.0), 0, 1, 0);

}


// Este telescopio sirve para apuntar desde un planeta a uno de sus satelites (p.e. para apuntar desde la Tierra a la Luna)
void myTelescopio2(float distancia, float angulo, float distancia_obj, float angulo_obj, int width, int height) {
	float x, y;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (GLfloat)width / (GLfloat)height, NEAR_TELESCOPIO, FAR_TELESCOPIO);
	x = distancia * cos(angulo * PI / 180.0);
	y = -1 * distancia * sin(angulo * PI / 180.0);
	gluLookAt(x, 0, y, x + distancia_obj * cos((angulo + angulo_obj) * PI / 180.0), 0,
		y - distancia_obj * sin((angulo + angulo_obj) * PI / 180.0), 0, 1, 0);

}

// Este telescopio sirve para apuntar desde un satélite al planeta que orbita (p.e. para apuntar desde la Luna a la Tierra)
void myTelescopio3(float distancia, float angulo, float distancia_obj, float angulo_obj, int width, int height) {

	float x, y;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (GLfloat)width / (GLfloat)height, NEAR_TELESCOPIO, FAR_TELESCOPIO);
	x = distancia * cos(angulo * PI / 180.0);
	y = -1 * distancia * sin(angulo * PI / 180.0);
	gluLookAt(x + distancia_obj * cos((angulo + angulo_obj) * PI / 180.0), 0,
		y - distancia_obj * sin((angulo + angulo_obj) * PI / 180.0), x, 0, y, 0, 1, 0);

}

int orbitasActivadas() {
	return orbitas;
}

/**************************************
FUNCION DE CONTROL DE TECLAS ESPECIAIS:
**************************************
*/
void myTeclasespeciales(int cursor, int x, int y)
{
	switch (cursor)
	{
		//Giros:
	case GLUT_KEY_UP:
		beta += INCREMENTO;
		break;
	case GLUT_KEY_DOWN:
		beta -= INCREMENTO;
		break;
	case GLUT_KEY_RIGHT:
		alpha += INCREMENTO;
		break;
	case GLUT_KEY_LEFT:
		alpha -= INCREMENTO;
		break;

	default:
		break;
	}

	glutPostRedisplay();
}


void myTeclado(unsigned char tras, int x, int y)
{
	switch (tras) {
	case 'o':
		if (orbitas == 0) {
			orbitas = 1;
		}
		else {
			orbitas = 0;
		}

		break;
	case '+':
		distancia = fmax(2000.0f, distancia - 10);
		break;
	case '-':
		distancia = fmin(4000.0f, distancia + 10);
		break;
	default:
		break;
	}

	glutPostRedisplay();
}