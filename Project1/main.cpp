#include <GL/glut.h>  
#include <math.h>

GLfloat moveX = 0.0f;
GLfloat moveY = 0.0f;
GLfloat moveZ = 0.0f;

GLfloat rotX = 0.0f;
GLfloat rotY = 0.0f;
GLfloat rotZ = 0.0f;

GLfloat camY = 0.0f;
GLfloat camX = 0.0f;
GLfloat camZ = 0.0f;

GLUquadricObj* qobj;

void init() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	qobj = gluNewQuadric();
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

void drawCylinder(GLdouble base, GLdouble top, GLdouble height, GLdouble slices, GLdouble stacks) {
	glPushMatrix();
	// base
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(1.0, 0.5, 0.5);
	glVertex3f(0.0, 0.0, 0.0);
	for (float i = 0; i < 360; i += 1.2) {
		glVertex3f(base * cos(i), base * sin(i), 0.0);
		glVertex3f(base * cos(i + 0.2), base * sin(i + 0.2), 0.0);
	}
	glEnd();
	// cylinder
	glColor3f(0.5, 0.0, 1.0);
	gluCylinder(qobj, base, top, height, slices, stacks);
	// top
	glTranslatef(0.0, 0.0, height);
	glColor3f(1.0, 1.0, 0.5);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0.0, 0.0, 0.0);
	for (float i = 0; i < 360; i += 0.2) {
		glVertex3f(top * cos(i), top * sin(i), 0.0);
		glVertex3f(top * cos(i + 0.2), top * sin(i + 0.2), 0.0);
	}
	glEnd();
	glPopMatrix();
}

void drawCube(GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLfloat h, GLfloat l) {
	glBegin(GL_QUADS);

	// front
	glColor3f(1.0, 0.5, 0.5);
	glVertex3f(x, y, z + l);
	glVertex3f(x + w, y, z + l);
	glVertex3f(x + w, y + h, z + l);
	glVertex3f(x, y + h, z + l);

	// top
	glColor3f(1.0, 1.0, 0.5);
	glVertex3f(x, y + h, z);
	glVertex3f(x, y + h, z + l);
	glVertex3f(x + w, y + h, z + l);
	glVertex3f(x + w, y + h, z);

	// back
	glColor3f(1.0, 0.5, 1.0);
	glVertex3f(x, y, z);
	glVertex3f(x, y + h, z);
	glVertex3f(x + w, y + h, z);
	glVertex3f(x + w, y, z);

	// bottom
	glColor3f(0.5, 1.0, 1.0);
	glVertex3f(x, y, z);
	glVertex3f(x + w, y, z);
	glVertex3f(x + w, y, z + l);
	glVertex3f(x, y, z + l);

	// left
	glColor3f(0.5, 1.0, 0.5);
	glVertex3f(x, y, z);
	glVertex3f(x, y, z + l);
	glVertex3f(x, y + h, z + l);
	glVertex3f(x, y + h, z);

	// right
	glColor3f(0.5, 0.5, 1.0);
	glVertex3f(x + w, y, z);
	glVertex3f(x + w, y + h, z);
	glVertex3f(x + w, y + h, z + l);
	glVertex3f(x + w, y, z + l);
	glEnd();
}

void drawRailway(GLfloat len) {
	drawCube(0.3, 0.5, 0, 0.3, 0.3, len);
	drawCube(3.7, 0.5, 0, 0.3, 0.3, len);
	for (int i = 0; i < len; i += 2) {
		drawCube(-0.3, 0, i, 5, 0.3, 0.8);
	}
}

void drawSignalPost() {
	glPushMatrix();
	glRotatef(90, -1, 0, 0);
	gluCylinder(gluNewQuadric(), 0.4, 0.4, 10, 200, 200);

	glPushMatrix();
	glTranslatef(0, 0.5, 10.8);
	glRotatef(90, -1, 0, 0);
	drawCylinder(0.5, 0.5, 0.1, 200, 200);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0.5, 12);
	glRotatef(90, -1, 0, 0);
	drawCylinder(0.5, 0.5, 0.1, 200, 200);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0.5, 13.2);
	glRotatef(90, -1, 0, 0);
	drawCylinder(0.5, 0.5, 0.1, 200, 200);
	glPopMatrix();

	drawCube(-1, -0.5, 10, 2, 1, 4);
	glPopMatrix();
}

void drawTrainCoach(GLfloat x, GLfloat y, GLfloat z) {
	glPushMatrix();
	glTranslatef(0.0 + x, 0.0 + y, 0.1 + z);
	glRotatef(90, 0, 1, 0);
	drawCylinder(0.1, 0.1, 0.05, 50, 50);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.275 + x, 0.0 + y, 0.1 + z);
	glRotatef(90, 0, 1, 0);
	drawCylinder(0.1, 0.1, 0.05, 50, 50);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0 + x, 0.0 + y, 0.3 + z);
	glRotatef(90, 0, 1, 0);
	drawCylinder(0.1, 0.1, 0.05, 50, 50);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.275 + x, 0.0 + y, 0.3 + z);
	glRotatef(90, 0, 1, 0);
	drawCylinder(0.1, 0.1, 0.05, 50, 50);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0 + x, 0.0 + y, 1.25 + z);
	glRotatef(90, 0, 1, 0);
	drawCylinder(0.1, 0.1, 0.05, 50, 50);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.275 + x, 0.0 + y, 1.25 + z);
	glRotatef(90, 0, 1, 0);
	drawCylinder(0.1, 0.1, 0.05, 50, 50);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0 + x, 0.0 + y, 1.05 + z);
	glRotatef(90, 0, 1, 0);
	drawCylinder(0.1, 0.1, 0.05, 50, 50);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.275 + x, 0.0 + y, 1.05 + z);
	glRotatef(90, 0, 1, 0);
	drawCylinder(0.1, 0.1, 0.05, 50, 50);
	glPopMatrix();

	drawCube(0.0 + x, 0.1 + y, 0.0 + z, 0.325, 0.325, 1.35);
}

void drawTrain(int len) {
	for (int i = 0; i < len; i++) {
		drawTrainCoach(0.0, 0.0, 1.45 * i);
	}
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	gluLookAt(0.0 + camX, 2.0 + camY, 5.0 + camZ, 0, 0, 0, 0, 1.0, 0);

	glTranslatef(moveX, moveY, moveZ);

	glRotatef(rotX, 1.0f, 0.0f, 0.0f);
	glRotatef(rotY, 0.0f, 1.0f, 0.0f);
	glRotatef(rotZ, 0.0f, 0.0f, 1.0f);

	drawGrid();

	drawAxes();

	//drawRailway(100);
	//drawSignalPost();
	drawTrain(5);

	glPopMatrix();

	glutSwapBuffers();
}

void keyboardSpecial(int key, int x, int y) {
	if (key == GLUT_KEY_UP)
		moveZ += 1;

	if (key == GLUT_KEY_DOWN)
		moveZ -= 1;

	if (key == GLUT_KEY_LEFT)
		moveX += 1;

	if (key == GLUT_KEY_RIGHT)
		moveX -= 1;

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

	glutPostRedisplay();
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
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(150, 150);
	glutCreateWindow("Railway Crossing");
	glutDisplayFunc(display);
	glutReshapeFunc(changeSize);

	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboardSpecial);

	init();
	glutMainLoop();
	return 0;
}