#ifndef LODS_H
#define LODS_H

#define GL_CLAMP_TO_EDGE 0x812F
#include <SFML/OpenGL.hpp>

const float PI = 3.141592653; 
bool mass[1000][1000][1000];
float size = 20.f;

GLuint LoadTexture(sf::String path)
{
		sf::Image image;
		if (!image.loadFromFile(path))
            return EXIT_FAILURE;     // if path is fake
		image.flipVertically(); //image flip, bc opengl use 0, 0 => 1, 1 and use flips the texture
			
		GLuint texture = 0;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, image.getSize().x, image.getSize().y, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // hiding the edges of skybox
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		return texture;
}

void createBlock(GLuint block[], float size)
{
	//Back :
	glBindTexture(GL_TEXTURE_2D, block[0]);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex3f(-size, -size, -size);
		glTexCoord2f(1, 0); glVertex3f( size, -size, -size);
		glTexCoord2f(1, 1); glVertex3f( size,  size, -size);
		glTexCoord2f(0, 1); glVertex3f(-size,  size, -size);
	glEnd();

	//Front :
	glBindTexture(GL_TEXTURE_2D, block[1]);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex3f( size, -size,  size);
		glTexCoord2f(1, 0); glVertex3f(-size, -size,  size);
		glTexCoord2f(1, 1); glVertex3f(-size,  size,  size);
		glTexCoord2f(0, 1); glVertex3f( size,  size,  size);
	glEnd();

	//Left :
	glBindTexture(GL_TEXTURE_2D, block[2]);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex3f(-size, -size,  size);
		glTexCoord2f(1, 0); glVertex3f(-size, -size, -size);
		glTexCoord2f(1, 1); glVertex3f(-size,  size, -size);
		glTexCoord2f(0, 1); glVertex3f(-size,  size,  size);
	glEnd();

	//Right :
	glBindTexture(GL_TEXTURE_2D, block[3]);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex3f( size, -size, -size);
		glTexCoord2f(1, 0); glVertex3f( size, -size,  size);
		glTexCoord2f(1, 1); glVertex3f( size,  size,  size);
		glTexCoord2f(0, 1); glVertex3f( size,  size, -size);
	glEnd();

	//Bottom :
	glBindTexture(GL_TEXTURE_2D, block[4]);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex3f(-size, -size,  size);
		glTexCoord2f(1, 0); glVertex3f( size, -size,  size);
		glTexCoord2f(1, 1); glVertex3f( size, -size, -size);
		glTexCoord2f(0, 1); glVertex3f(-size, -size, -size);
	glEnd();

	//Top :
	glBindTexture(GL_TEXTURE_2D, block[5]);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex3f(-size,  size, -size);
		glTexCoord2f(1, 0); glVertex3f( size,  size, -size);
		glTexCoord2f(1, 1); glVertex3f( size,  size,  size);
		glTexCoord2f(0, 1); glVertex3f(-size,  size,  size);
	glEnd();
}

bool check(int x, int y, int z)
{
	if (x < 0 || x >= 1000 || y < 0 || y >= 1000 || z < 0 || x >= 1000)
		return false;
	return mass[x][y][z];
}

#endif LODS_H