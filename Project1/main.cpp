#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <GL/glut.h>

#define PI 3.1415927
#define TX 23
using namespace std;

GLfloat moveX = -7.0f;
GLfloat moveY = 0.0f;
GLfloat moveZ = -36.0f;

GLfloat rotX = 0.0f;
GLfloat rotY = -87.0f;
GLfloat rotZ = 0.0f;

GLfloat camY = 4.5f;
GLfloat camX = 0.0f;
GLfloat camZ = 5.0f;

GLfloat trainPos = -200.0f;
GLfloat carX = 30.0f;
GLfloat carY = -10.0f;

int light = 3;
bool gateOpened = true;

GLUquadricObj* qobj;

static unsigned int texture[TX];

struct BitMapFile
{
	int sizeX;
	int sizeY;
	unsigned char* data;
};

BitMapFile* getbmp(string filename)
{
	int offset, headerSize;

	BitMapFile* bmpRGB = new BitMapFile;
	BitMapFile* bmpRGBA = new BitMapFile;

	ifstream infile(filename.c_str(), ios::binary);

	infile.seekg(10);
	infile.read((char*)&offset, 4);

	infile.read((char*)&headerSize, 4);

	infile.seekg(18);
	infile.read((char*)&bmpRGB->sizeX, 4);
	infile.read((char*)&bmpRGB->sizeY, 4);

	int padding = (3 * bmpRGB->sizeX) % 4 ? 4 - (3 * bmpRGB->sizeX) % 4 : 0;
	int sizeScanline = 3 * bmpRGB->sizeX + padding;
	int sizeStorage = sizeScanline * bmpRGB->sizeY;
	bmpRGB->data = new unsigned char[sizeStorage];

	infile.seekg(offset);
	infile.read((char*)bmpRGB->data, sizeStorage);

	int startScanline, endScanlineImageData, temp;
	for (int y = 0; y < bmpRGB->sizeY; y++)
	{
		startScanline = y * sizeScanline;
		endScanlineImageData = startScanline + 3 * bmpRGB->sizeX;
		for (int x = startScanline; x < endScanlineImageData; x += 3)
		{
			temp = bmpRGB->data[x];
			bmpRGB->data[x] = bmpRGB->data[x + 2];
			bmpRGB->data[x + 2] = temp;
		}
	}

	bmpRGBA->sizeX = bmpRGB->sizeX;
	bmpRGBA->sizeY = bmpRGB->sizeY;
	bmpRGBA->data = new unsigned char[4 * bmpRGB->sizeX * bmpRGB->sizeY];

	for (int j = 0; j < 4 * bmpRGB->sizeY * bmpRGB->sizeX; j += 4)
	{
		bmpRGBA->data[j] = bmpRGB->data[(j / 4) * 3];
		bmpRGBA->data[j + 1] = bmpRGB->data[(j / 4) * 3 + 1];
		bmpRGBA->data[j + 2] = bmpRGB->data[(j / 4) * 3 + 2];
		bmpRGBA->data[j + 3] = 0xFF;
	}
	return bmpRGBA;
}

void loadExternalTextures()
{
	BitMapFile* image[TX];
	image[0] = getbmp("res/metal1.bmp");
	image[1] = getbmp("res/metal2.bmp");
	image[2] = getbmp("res/wood.bmp");
	image[3] = getbmp("res/train1.bmp");
	image[4] = getbmp("res/train2.bmp");
	image[5] = getbmp("res/wheel1.bmp");
	image[6] = getbmp("res/wheel2.bmp");
	image[7] = getbmp("res/tyre.bmp");
	image[8] = getbmp("res/car1.bmp");
	image[9] = getbmp("res/car2.bmp");
	image[10] = getbmp("res/car3.bmp");
	image[11] = getbmp("res/car4.bmp");
	image[12] = getbmp("res/car5.bmp");
	image[13] = getbmp("res/car6.bmp");
	image[14] = getbmp("res/plastic.bmp");
	image[15] = getbmp("res/light1.bmp");
	image[16] = getbmp("res/light2.bmp");
	image[17] = getbmp("res/light3.bmp");
	image[18] = getbmp("res/bark.bmp");
	image[19] = getbmp("res/pine.bmp");
	image[20] = getbmp("res/road.bmp");
	image[21] = getbmp("res/grass.bmp");
	image[22] = getbmp("res/gate.bmp");

	for (int i = 0; i < TX; i++) {
		glBindTexture(GL_TEXTURE_2D, texture[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[i]->sizeX, image[i]->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, image[i]->data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
}

void initLighting() {
	GLfloat specularReflectance[] = { 1.0, 1.0, 1.0, 1.0 };
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularReflectance);
	glMateriali(GL_FRONT, GL_SHININESS, 30);

	// Main
	GLfloat L0_Ambient[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat L0_Diffuse[] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat L0_postion[] = { 0.0, 20.0, 0.0, 1.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, L0_Ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, L0_Diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, L0_postion);

	//Red
	GLfloat L2_Diffuse[] = { 1.0, 0.0, 0.0, 1.0 };
	GLfloat L2_Specular[] = { 1.0, 0.0, 0.0, 1.0 };
	GLfloat L2_postion[] = { 20, 5.3, -0.6, 1.0 };
	GLfloat L2_SpotDirection[] = { 1.0, 0.0, 0.0 };

	glLightfv(GL_LIGHT2, GL_DIFFUSE, L2_Diffuse);
	glLightfv(GL_LIGHT2, GL_SPECULAR, L2_Specular);
	glLightfv(GL_LIGHT2, GL_POSITION, L2_postion);

	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, L2_SpotDirection);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 30.0);

	//Yellow
	GLfloat L3_Diffuse[] = { 1.0, 1.0, 0.0, 1.0 };
	GLfloat L3_Specular[] = { 1.0, 1.0, 0.0, 1.0 };
	GLfloat L3_postion[] = { 20, 5.3, -0.6, 1.0 };
	GLfloat L3_SpotDirection[] = { 1.0, 0.0, 0.0 };

	glLightfv(GL_LIGHT3, GL_DIFFUSE, L3_Diffuse);
	glLightfv(GL_LIGHT3, GL_SPECULAR, L3_Specular);
	glLightfv(GL_LIGHT3, GL_POSITION, L3_postion);

	glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, L3_SpotDirection);
	glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 30.0);

	//Green
	GLfloat L4_Diffuse[] = { 0.0, 1.0, 0.0, 1.0 };
	GLfloat L4_Specular[] = { 0.0, 1.0, 0.0, 1.0 };
	GLfloat L4_postion[] = { 20, 5.3, -0.6, 1.0 };
	GLfloat L4_SpotDirection[] = { 1.0, 0.0, 0.0 };

	glLightfv(GL_LIGHT4, GL_DIFFUSE, L4_Diffuse);
	glLightfv(GL_LIGHT4, GL_SPECULAR, L4_Specular);
	glLightfv(GL_LIGHT4, GL_POSITION, L4_postion);

	glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, L4_SpotDirection);
	glLightf(GL_LIGHT4, GL_SPOT_CUTOFF, 30.0);

	glEnable(GL_LIGHT0);
}

void init() {
	glClearColor(0.35f, 0.35f, 0.35f, 1.0f);
	GLfloat globalAmbient[] = { 0.1, 0.1, 0.1, 1.0 };

	glGenTextures(TX, texture);
	loadExternalTextures();

	qobj = gluNewQuadric();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);


	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND);

	glEnable(GL_LIGHTING);

	glShadeModel(GL_SMOOTH);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE);
}

void drawAxes() {
	glBegin(GL_LINES);

	glLineWidth(1.5);

	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(-20, 0, 0);
	glVertex3f(20, 0, 0);

	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0, -20, 0);
	glVertex3f(0, 20, 0);

	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0, 0, -20);
	glVertex3f(0, 0, 20);

	glEnd();
}

void drawGrid() {
	GLfloat ext = 20.0f;
	GLfloat step = 1.0f;
	GLfloat yGrid = -0.4f;
	GLint line;

	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINES);
	for (line = -ext; line <= ext; line += step) {
		glVertex3f(line, yGrid, ext);
		glVertex3f(line, yGrid, -ext);

		glVertex3f(ext, yGrid, line);
		glVertex3f(-ext, yGrid, line);
	}
	glEnd();
}

void drawCylinder(GLdouble radius, GLdouble height, GLdouble slices, GLdouble stacks, int txBase, int txTop, int txCyl) {
	GLfloat y = 0.0, z = 0.0;
	GLfloat txX = 0.0, txY = 0.0;
	GLfloat xcos = 0.0, ysin = 0.0;
	GLfloat angle_stepsize = 0.1;

	glEnable(GL_TEXTURE_2D);

	// TUBE
	glBindTexture(GL_TEXTURE_2D, texture[txCyl]);

	glBegin(GL_QUAD_STRIP);
	for (GLfloat angle = 2 * PI; angle >= 0.0; angle = angle - angle_stepsize) {
		y = radius * sin(angle);
		z = radius * cos(angle);
		glTexCoord2f(1.0, 0.0);  glVertex3f(height, y, z);
		glTexCoord2f(1.0, 1.0);  glVertex3f(0.0, y, z);
	}
	glVertex3f(height, 0.0, radius);
	glVertex3f(0.0, 0.0, radius);
	glEnd();

	// BASE
	glBindTexture(GL_TEXTURE_2D, texture[txBase]);

	glBegin(GL_POLYGON);
	for (GLfloat angle = 0.0; angle < 2 * PI; angle = angle + angle_stepsize) {
		ysin = sin(angle);
		xcos = cos(angle);
		y = radius * ysin;
		z = radius * xcos;
		txX = xcos * 0.5 + 0.5;
		txY = ysin * 0.5 + 0.5;

		glTexCoord2f(txX, txY); glVertex3f(0.0, y, z);
	}
	glVertex3f(0.0, 0.0, radius);
	glEnd();

	// TOP
	glBindTexture(GL_TEXTURE_2D, texture[txTop]);

	glBegin(GL_POLYGON);
	for (GLfloat angle = 2 * PI - angle_stepsize; angle >= 0.0; angle = angle - angle_stepsize) {
		ysin = sin(angle);
		xcos = cos(angle);
		y = radius * ysin;
		z = radius * xcos;
		txX = xcos * 0.5 + 0.5;
		txY = ysin * 0.5 + 0.5;

		glTexCoord2f(txX, txY); glVertex3f(height, y, z);
	}
	glVertex3f(height, 0.0, radius);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void drawCube(GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLfloat h, GLfloat l, int txFront, int txTop, int txBack, int txBottom, int txLeft, int txRight) {
	glEnable(GL_TEXTURE_2D);

	// front
	glNormal3f(0.0, 0.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, texture[txFront]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(x, y, z + l);
	glTexCoord2f(1.0, 0.0); glVertex3f(x + w, y, z + l);
	glTexCoord2f(1.0, 1.0); glVertex3f(x + w, y + h, z + l);
	glTexCoord2f(0.0, 1.0); glVertex3f(x, y + h, z + l);
	glEnd();

	// top
	glNormal3f(0.0, 1.0, 0.0);
	glBindTexture(GL_TEXTURE_2D, texture[txTop]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(x, y + h, z);
	glTexCoord2f(1.0, 0.0); glVertex3f(x, y + h, z + l);
	glTexCoord2f(1.0, 1.0); glVertex3f(x + w, y + h, z + l);
	glTexCoord2f(0.0, 1.0); glVertex3f(x + w, y + h, z);
	glEnd();

	// back
	glNormal3f(0.0, 0.0, -1.0);
	glBindTexture(GL_TEXTURE_2D, texture[txBack]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(x, y, z);
	glTexCoord2f(0.0, 1.0); glVertex3f(x, y + h, z);
	glTexCoord2f(1.0, 1.0); glVertex3f(x + w, y + h, z);
	glTexCoord2f(1.0, 0.0); glVertex3f(x + w, y, z);
	glEnd();

	// bottom
	glNormal3f(0.0, -1.0, 0.0);
	glBindTexture(GL_TEXTURE_2D, texture[txBottom]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(x, y, z);
	glTexCoord2f(0.0, 1.0); glVertex3f(x + w, y, z);
	glTexCoord2f(1.0, 1.0); glVertex3f(x + w, y, z + l);
	glTexCoord2f(1.0, 0.0); glVertex3f(x, y, z + l);
	glEnd();

	// left
	glNormal3f(-1.0, 0.0, 0.0);
	glBindTexture(GL_TEXTURE_2D, texture[txLeft]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(x, y, z);
	glTexCoord2f(1.0, 0.0); glVertex3f(x, y, z + l);
	glTexCoord2f(1.0, 1.0); glVertex3f(x, y + h, z + l);
	glTexCoord2f(0.0, 1.0); glVertex3f(x, y + h, z);
	glEnd();

	// right
	glNormal3f(1.0, 0.0, 0.0);
	glBindTexture(GL_TEXTURE_2D, texture[txRight]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(x + w, y, z);
	glTexCoord2f(0.0, 1.0); glVertex3f(x + w, y + h, z);
	glTexCoord2f(1.0, 1.0); glVertex3f(x + w, y + h, z + l);
	glTexCoord2f(1.0, 0.0); glVertex3f(x + w, y, z + l);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void drawRailway(GLfloat len) {
	drawCube(0.3, 0.5, 0, 0.3, 0.3, len, 0, 0, 0, 0, 0, 0);
	drawCube(3.7, 0.5, 0, 0.3, 0.3, len, 0, 0, 0, 0, 0, 0);
	for (int i = 0; i < len; i += 2) {
		drawCube(-0.3, 0, i, 5, 0.3, 0.6, 2, 2, 2, 2, 2, 2);
	}
}

void drawSignalPost() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[14]);

	glPushMatrix();
	glRotatef(90, -1, 0, 0);
	gluCylinder(gluNewQuadric(), 0.4, 0.4, 10, 200, 200);

	glPushMatrix();
	glTranslatef(0, 0.5, 10.8);
	glRotatef(90, 0, 0, 1);
	drawCylinder(0.5, 0.1, 200, 200, 14, (light == 3) ? 17 : 14, 14);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0.5, 12);
	glRotatef(90, 0, 0, 1);
	drawCylinder(0.5, 0.1, 200, 200, 14, (light == 2) ? 16 : 14, 14);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0.5, 13.2);
	glRotatef(90, 0, 0, 1);
	drawCylinder(0.5, 0.1, 200, 200, 14, (light == 1) ? 15 : 14, 14);
	glPopMatrix();

	drawCube(-1, -0.5, 10, 2, 1, 4, 14, 14, 14, 14, 14, 14);
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}

void drawCar() {
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();

	// Sides - Windows
	glBindTexture(GL_TEXTURE_2D, texture[8]);
	glBegin(GL_TRIANGLE_FAN);
	glTexCoord2f(0.0, 0.0); glVertex3f(0.24, 0.2, 0.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(0.36, 0.4, 0.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(0.84, 0.4, 0.0);
	glTexCoord2f(0.0, 1.0); glVertex3f(0.96, 0.2, 0.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[8]);
	glBegin(GL_TRIANGLE_FAN);
	glTexCoord2f(0.0, 0.0); glVertex3f(0.24, 0.2, 0.4);
	glTexCoord2f(1.0, 0.0); glVertex3f(0.96, 0.2, 0.4);
	glTexCoord2f(1.0, 1.0); glVertex3f(0.84, 0.4, 0.4);
	glTexCoord2f(0.0, 1.0); glVertex3f(0.36, 0.4, 0.4);
	glEnd();

	// Top
	glBindTexture(GL_TEXTURE_2D, texture[9]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(0.36, 0.4, 0.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(0.36, 0.4, 0.4);
	glTexCoord2f(1.0, 1.0); glVertex3f(0.84, 0.4, 0.4);
	glTexCoord2f(0.0, 1.0); glVertex3f(0.84, 0.4, 0.0);
	glEnd();

	// Windscreen
	glBindTexture(GL_TEXTURE_2D, texture[8]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(0.24, 0.2, 0.4);
	glTexCoord2f(1.0, 0.0); glVertex3f(0.36, 0.4, 0.4);
	glTexCoord2f(1.0, 1.0); glVertex3f(0.36, 0.4, 0.0);
	glTexCoord2f(0.0, 1.0); glVertex3f(0.24, 0.2, 0.0);
	glEnd();

	// Back Window
	glBindTexture(GL_TEXTURE_2D, texture[8]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(0.84, 0.4, 0.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(0.84, 0.4, 0.4);
	glTexCoord2f(1.0, 1.0); glVertex3f(0.96, 0.2, 0.4);
	glTexCoord2f(0.0, 1.0); glVertex3f(0.96, 0.2, 0.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	// Body
	drawCube(0.0, 0.0, 0.0, 1.2, 0.2, 0.4, 13, 9, 13, 12, 10, 11);

	// Wheels
	glPushMatrix();
	glTranslatef(0.2, 0.0, 0.02);
	glRotatef(90.0, 0, 1, 0);
	drawCylinder(0.1, 0.05, 50, 50, 6, 6, 7);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.2, 0.0, 0.42);
	glRotatef(90.0, 0, 1, 0);
	drawCylinder(0.1, 0.05, 50, 50, 6, 6, 7);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.9, 0.0, 0.02);
	glRotatef(90.0, 0, 1, 0);
	drawCylinder(0.1, 0.05, 50, 50, 6, 6, 7);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.9, 0.0, 0.42);
	glRotatef(90.0, 0, 1, 0);
	drawCylinder(0.1, 0.05, 50, 50, 6, 6, 7);
	glPopMatrix();

	glPopMatrix();
}

void drawTrainCoach(GLfloat x, GLfloat y, GLfloat z) {
	glPushMatrix();
	glTranslatef(x, y, 0.1 + z);
	drawCylinder(0.08, 0.05, 50, 50, 5, 5, 1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.3 + x, y, 0.1 + z);
	drawCylinder(0.08, 0.05, 50, 50, 5, 5, 1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(x, y, 0.26 + z);
	drawCylinder(0.08, 0.05, 50, 50, 5, 5, 1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.3 + x, y, 0.26 + z);
	drawCylinder(0.08, 0.05, 50, 50, 5, 5, 1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(x, y, 1.25 + z);
	drawCylinder(0.08, 0.05, 50, 50, 5, 5, 1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.3 + x, y, 1.25 + z);
	drawCylinder(0.08, 0.05, 50, 50, 5, 5, 1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(x, y, 1.09 + z);
	drawCylinder(0.08, 0.05, 50, 50, 5, 5, 1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.3 + x, y, 1.09 + z);
	drawCylinder(0.08, 0.05, 50, 50, 5, 5, 1);
	glPopMatrix();

	drawCube(0.0 + x, 0.08 + y, z, 0.35, 0.325, 1.35, 3, 4, 3, 1, 3, 3);
}

void drawTrain(int len) {
	for (int i = 0; i < len; i++) {
		drawTrainCoach(0.0, 0.0, 1.45 * i);
	}
}

void drawTree() {
	glEnable(GL_TEXTURE_2D);

	glPushMatrix();
	glRotatef(90, -1, 0, 0);

	// Trunk (Cylinder)
	glBindTexture(GL_TEXTURE_2D, texture[18]);
	GLUquadric* qobj = gluNewQuadric();
	gluQuadricTexture(qobj, GL_TRUE);
	gluCylinder(qobj, 0.4, 0.4, 3.0, 50, 50);
	gluDeleteQuadric(qobj);

	// Leaves (Cone)
	glTranslatef(0.0, 0.0, 3.0);
	glBindTexture(GL_TEXTURE_2D, texture[19]);
	qobj = gluNewQuadric();
	gluQuadricTexture(qobj, GL_TRUE);
	gluCylinder(qobj, 2.0, 0.0, 5.0, 50, 50);
	gluDeleteQuadric(qobj);

	// Lower (Circle)
	glPushMatrix();
	glRotatef(90, 0, -1, 0);
	glBindTexture(GL_TEXTURE_2D, texture[19]);

	glBegin(GL_POLYGON);
	for (GLfloat angle = 0.0; angle < 2 * PI; angle = angle + 0.1) {
		GLfloat ysin = sin(angle);
		GLfloat xcos = cos(angle);
		GLfloat y = 2.0 * ysin;
		GLfloat z = 2.0 * xcos;
		GLfloat txX = xcos * 0.5 + 0.5;
		GLfloat txY = ysin * 0.5 + 0.5;

		glTexCoord2f(txX, txY); glVertex3f(0.0, y, z);
	}
	glVertex3f(0.0, 0.0, 2.0);
	glEnd();
	glPopMatrix();

	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}

void drawGate(bool isOpened) {
	drawCube(0.0, 0.0, 0.0, 0.8, 2.0, 0.8, 0, 0, 0, 0, 0, 0);

	glPushMatrix();
	if (isOpened) {
		glRotatef(-90.0, 1, 0, 0);
		glTranslatef(0.0, -2.0, 0.0);
	}
	drawCube(0.3, 1.4, 0.5, 0.2, 0.5, 18.0, 22, 22, 22, 22, 22, 22);
	glPopMatrix();
}

void drawRoad() {
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, texture[20]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(-80.0, 0.0, -15.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(-80.0, 0.0, -5.0);
	glTexCoord2f(1.0, 40.0); glVertex3f(80.0, 0.0, -5.0);
	glTexCoord2f(0.0, 40.0); glVertex3f(80.0, 0.0, -15.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[21]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(-100.0, -0.001, -100.0);
	glTexCoord2f(10.0, 0.0); glVertex3f(-100.0, -0.001, 100.0);
	glTexCoord2f(10.0, 10.0); glVertex3f(100.0, -0.001, 100.0);
	glTexCoord2f(0.0, 10.0); glVertex3f(100.0, -0.001, -100.0);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	gluLookAt(camX, camY, camZ, 0, 0, 0, 0, 1.0, 0);

	glTranslatef(moveX, moveY, moveZ);

	glRotatef(rotX, 1.0f, 0.0f, 0.0f);
	glRotatef(rotY, 0.0f, 1.0f, 0.0f);
	glRotatef(rotZ, 0.0f, 0.0f, 1.0f);

	/*drawGrid();
	drawAxes();*/

	initLighting();

	drawRoad();

	glPushMatrix();
	glTranslatef(0.0, 0.0, -200.0);
	drawRailway(400);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.35, 1.6, trainPos);
	glScalef(10.0, 10.0, 10.0);
	drawTrain(5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(carX, 0.6, carY);
	glScalef(6.5, 6.0, 6.5);
	drawCar();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(20.0, 0.0, 0.0);
	glScalef(0.5, 0.5, 0.5);
	glRotatef(-90.0, 0, 1, 0);
	drawSignalPost();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(30.0, 0.0, 5.0);
	glScalef(1.5, 2.0, 1.5);
	drawTree();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(15.0, 0.0, -20.0);
	drawGate(gateOpened);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 0.0, -100.0);
	for (int i = 0; i < 5; i++) {
		glPushMatrix();
		glTranslatef(0.0, 0.0, 20.0  * i);

		glPushMatrix();
		glTranslatef(10.0, 0.0, 0.0);
		glScalef(1.5, 2.0, 1.5);
		drawTree();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-5.0, 0.0, 0.0);
		glScalef(1.5, 2.0, 1.5);
		drawTree();
		glPopMatrix();

		glPopMatrix();
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 0.0, 5.0);
	for (int i = 0; i < 5; i++) {
		glPushMatrix();
		glTranslatef(0.0, 0.0, 20.0  * i);

		glPushMatrix();
		glTranslatef(10.0, 0.0, 0.0);
		glScalef(1.5, 2.0, 1.5);
		drawTree();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-5.0, 0.0, 0.0);
		glScalef(1.5, 2.0, 1.5);
		drawTree();
		glPopMatrix();

		glPopMatrix();
	}
	glPopMatrix();

	glPopMatrix();
	glutSwapBuffers();
}

void keyboardSpecial(int key, int x, int y) {
	if (key == GLUT_KEY_DOWN) {
		carX += 1;
	}
	else if (key == GLUT_KEY_UP) {
		carX -= 1;
	}
	else if (key == GLUT_KEY_LEFT) {
		carY += 1;
	}
	else if (key == GLUT_KEY_RIGHT) {
		carY -= 1;
	}

	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
	if (key == 'w') {
		camY += 0.5;
	}
	else if (key == 's') {
		camY -= 0.5;
	}
	else if (key == 'a') {
		rotY += 3.0;
	}
	else if (key == 'd') {
		rotY -= 3.0;
	}
	if (key == '8') {
		moveZ += 1;
	}
	else if (key == '5') {
		moveZ -= 1;
	}
	else if (key == '4') {
		moveX += 1;
	}
	else if (key == '6') {
		moveX -= 1;
	}

	glutPostRedisplay();
}

void timer(int x) {
	if (trainPos > 150) {
		trainPos = -200;
	}
	else {
		trainPos += 1;

		if (trainPos <= -140 && light != 3) {
			light = 3;

			glDisable(GL_LIGHT2);
			glDisable(GL_LIGHT3);
			glEnable(GL_LIGHT4);
		}
		else if (trainPos > -140 && trainPos <= -125 && light != 2) {
			light = 2;

			glDisable(GL_LIGHT2);
			glEnable(GL_LIGHT3);
			glDisable(GL_LIGHT4);
		}
		else if (trainPos > -125 && trainPos <= 5 && light != 1) {
			light = 1;
			gateOpened = false;

			glEnable(GL_LIGHT2);
			glDisable(GL_LIGHT3);
			glDisable(GL_LIGHT4);
		}
		else if (trainPos > 5 && trainPos <= 20 && light != 2) {
			light = 2;

			glDisable(GL_LIGHT2);
			glEnable(GL_LIGHT3);
			glDisable(GL_LIGHT4);
		}
		else if (trainPos > 20 && light != 3) {
			light = 3;
			gateOpened = true;

			glDisable(GL_LIGHT2);
			glDisable(GL_LIGHT3);
			glEnable(GL_LIGHT4);
		}
	}

	glutPostRedisplay();
	glutTimerFunc(60, timer, 1);
}

void changeSize(GLsizei w, GLsizei h) {
	glViewport(0, 0, w, h);
	GLfloat aspect_ratio = h == 0 ? w / 1 : (GLfloat)w / (GLfloat)h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(120.0, aspect_ratio, 1.0, 100.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(150, 150);
	glutCreateWindow("Railway Crossing");
	glutDisplayFunc(display);
	glutReshapeFunc(changeSize);

	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboardSpecial);

	glutTimerFunc(60.0, timer, 1);
	init();
	glutMainLoop();
	return 0;
}