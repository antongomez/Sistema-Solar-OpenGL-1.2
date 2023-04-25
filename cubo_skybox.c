#include <glut.h>

#include "cubo_skybox.h"

int cubo_skyBox(int textura_milky, int textura_stars)
{
	int indice;
	indice = glGenLists(2);
	glNewList(indice, GL_COMPILE);

	glBindTexture(GL_TEXTURE_2D, textura_milky);
	glBegin(GL_QUADS);
	// Parte de atras do cubo
	glTexCoord2f(0.0, 0.0); glVertex3f(-1.0, -1.0, -1.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(1.0, -1.0, -1.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(1.0, 1.0, -1.0);
	glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, 1.0, -1.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, textura_stars);
	glBegin(GL_QUADS);
	// Parte de arriba
	glTexCoord2f(0.0, 0.0); glVertex3f(-1.0, 1.0, -1.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(1.0, 1.0, -1.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(1.0, 1.0, 1.0);
	glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, 1.0, 1.0);

	// Parte da dereita
	glTexCoord2f(0.0, 0.0); glVertex3f(1.0, -1.0, -1.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(1.0, -1.0, 1.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(1.0, 1.0, 1.0);
	glTexCoord2f(0.0, 1.0); glVertex3f(1.0, 1.0, -1.0);

	// Parte da esquerda
	glTexCoord2f(0.0, 0.0); glVertex3f(-1.0, -1.0, 1.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(-1.0, -1.0, -1.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(-1.0, 1.0, -1.0);
	glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, 1.0, 1.0);

	// Parte de abaixo
	glTexCoord2f(0.0, 0.0); glVertex3f(-1.0, -1.0, 1.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(1.0, -1.0, 1.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(1.0, -1.0, -1.0);
	glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, -1.0, -1.0);

	// Parte de diante do cubo
	glTexCoord2f(0.0, 0.0); glVertex3f(1.0, -1.0, 1.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(-1.0, -1.0, 1.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(-1.0, 1.0, 1.0);
	glTexCoord2f(0.0, 1.0); glVertex3f(1.0, 1.0, 1.0);

	glEnd();


	glEndList();

	return indice;
}