#include "definiciones.h"

int crearListaCubo(int textura)
{
	int indice = glGenLists(1);
	glNewList(indice, GL_COMPILE);

	glBindTexture(GL_TEXTURE_2D, textura);
	glBegin(GL_QUADS);
	// Parte de atras do cubo
	glTexCoord2f(0.0, 0.0); glVertex3f(-1.0, -1.0, -1.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(1.0, -1.0, -1.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(1.0, 1.0, -1.0);
	glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, 1.0, -1.0);

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