#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <GL/glut.h>
#include <GL/GLU.h>
#include <GL/GL.h>
#include <time.h>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>
#include "tinyxml2.h"
#include "Modelador.h"
#include <sstream>

using namespace std;

//x do seno, tamanho do pote calculado
//atrav�s do y do seno
float GLOBAL_height = 1, GLOBAL_angle = 0, GLOBAL_barrelroll = 0;
float posx = 0, posy = 0, posz = 0;
double size = 1;

void drawVertices(vector<GLfloat> vertices, int mode){
	if (mode == GL_QUADS){
		for (unsigned int i = 0, j = 0; i < vertices.size(); i += 3, j++){
			glBegin(mode);
			glNormal3f(vertices[i], vertices[i + 1], vertices[i + 2]);
			glVertex3f(vertices[i], vertices[i + 1], vertices[i + 2]);
			i += 3;
			glNormal3f(vertices[i], vertices[i + 1], vertices[i + 2]);
			glVertex3f(vertices[i], vertices[i + 1], vertices[i + 2]);
			i += 3;
			glNormal3f(vertices[i], vertices[i + 1], vertices[i + 2]);
			glVertex3f(vertices[i], vertices[i + 1], vertices[i + 2]);
			i += 3;
			glNormal3f(vertices[i], vertices[i + 1], vertices[i + 2]);
			glVertex3f(vertices[i], vertices[i + 1], vertices[i + 2]);
			glEnd();
			glColor3f(rand() / ((float)RAND_MAX + 1), rand() / ((float)RAND_MAX + 1), rand() / ((float)RAND_MAX + 1));
		}
	}
	else{
		for (unsigned int i = 0, j = 0; i < vertices.size(); i += 3, j++){
			//multiplica-se por size, para aumentar o tamanho da esfera (relembrar coordenadas polares)
			glBegin(GL_TRIANGLES);
			glNormal3f(vertices[i] * size, vertices[i + 1] * size, vertices[i + 2] * size);
			glVertex3f(vertices[i] * size, vertices[i + 1] * size, vertices[i + 2] * size);
			i += 3;
			glNormal3f(vertices[i] * size, vertices[i + 1] * size, vertices[i + 2] * size);
			glVertex3f(vertices[i] * size, vertices[i + 1] * size, vertices[i + 2] * size);
			i += 3;
			glNormal3f(vertices[i] * size, vertices[i + 1] * size, vertices[i + 2] * size);
			glVertex3f(vertices[i] * size, vertices[i + 1] * size, vertices[i + 2] * size);
			glEnd();
			glColor3f(rand() / ((float)RAND_MAX + 1), rand() / ((float)RAND_MAX + 1), rand() / ((float)RAND_MAX + 1));
		}
	}
}

static void drawFromXML(tinyxml2::XMLElement *pElement, int mode){
	float x, y, z;
	const char *aux;
	std::vector<GLfloat> vertices;
	while (pElement != NULL) {
		aux = pElement->GetText();
		sscanf_s(aux, "X=%f Y=%f Z=%f", &x, &y, &z);
		vertices.push_back(x);
		vertices.push_back(y);
		vertices.push_back(z);
		pElement = pElement->NextSiblingElement("vertex");
	}
	drawVertices(vertices, mode);
}

void drawModel(const char *filename) {
	using namespace tinyxml2;
	tinyxml2::XMLDocument xmlDoc;
	XMLError eResult = xmlDoc.LoadFile(filename);
	XMLNode * pRoot = xmlDoc.FirstChild();
	if (pRoot == nullptr){
		return;
	}
	XMLElement *elem = pRoot->FirstChildElement("mode");
	if (elem == nullptr)
		drawFromXML(pRoot->FirstChildElement("vertex"), GL_TRIANGLES);
	else if (strcmp(elem->GetText(), "QUAD") == 0)
		drawFromXML(pRoot->FirstChildElement("vertex"), GL_QUADS);
}

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if (h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(45.0f, ratio, 1.0f, 1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

void renderScene(void) {
	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(0, 3, 5, 0, 0, 0, 0, 1, 0);

	glTranslatef(posx, posy, posz);
	glRotatef(GLOBAL_angle, 0, 1, 0);
	glRotatef(GLOBAL_barrelroll, 1, 0, 0);
	glShadeModel(GL_SMOOTH);
	drawModel("esfera.3d");

	// fim do frame
	glutSwapBuffers();
}

// escrever fun��o de processamento do teclado
void keyboardSpecial(int key, int x, int y){
	float step = 0.05;
	switch (key){
	case GLUT_KEY_UP:
		posz -= step;
		break;
	case GLUT_KEY_DOWN:
		posz += step;
		break;
	case GLUT_KEY_LEFT:
		posx -= step;
		break;
	case GLUT_KEY_RIGHT:
		posx += step;
		break;
	}
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y){
	float step = 0.05;
	switch (key){
	case '+':
		size += 0.05;
		break;
	case '-':
		size -= 0.05;
		break;
	case 'd':
		GLOBAL_angle += 1;
		break;
	case 'a':
		GLOBAL_angle -= 1;
	case 'w':
		GLOBAL_barrelroll += 1;
		break;
	case 's':
		GLOBAL_barrelroll -= 1;
		break;
	}
	glutPostRedisplay();
}


void init(void){
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	GLfloat light_position[] = { -4, 3, 3, 0 };
	glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
	glShadeModel(GL_FLAT);
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

// awkward main func do windows, just copy paste
int WINAPI WinMain(HINSTANCE inst, HINSTANCE prev, LPSTR cmd, int show) {
	srand(time(NULL));
	drawCubeXML(1);

	// init de cenas
	glutInit(&__argc, __argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(400, 400);
	glutCreateWindow("Modelador");
	//init();

	// registo de funcs
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutSpecialFunc(keyboardSpecial);
	glutKeyboardFunc(keyboard);

	// alguns settings para OpenGL
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CW);

	// entrar no loop do glut
	glutMainLoop();

	return 1;
}
