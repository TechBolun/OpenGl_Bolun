/*

	Bolun Gao
	
	*/
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "cyMatrix.h"
#include <iostream>
using namespace std;

const char* vertexShader = "shader.vs";
const char* fragmentShader = "shader.fs";

float colorR;
float colorG;
float colorB;

GLuint VBO;

/*
	set background color
*/
void myRender(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glClearColor(colorR, colorG, colorB, 0.0f);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableVertexAttribArray(0);

	glutSwapBuffers();
}

static void vertexBuffer() {
	float vertices[9] = {
		0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f
	};

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

void myKeyboard(unsigned char key, int x, int y) {
	if (key == 27) {
		exit(0);
	}
}

/*
	animate background color by using glutPostRedisplay
*/
void idle(int value) {

	// random num between 0 and 1
	colorR = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	colorG = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	colorB = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

	myRender();
	glutPostRedisplay();
	glutTimerFunc(33, idle, 1);
}

static void initialShader(GLuint shaderProgram, const char* shaderName, GLenum shaderType) {

}

int main(int argc, char *argv[]) {

	glutInit(&argc, argv);

	cout << "Bolun" << endl;

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(1024, 720);
	glutInitWindowPosition(100, 100);

	glutCreateWindow("OpenGL Window - Bolun Gao");

	glutDisplayFunc(myRender);

	if (glewInit() != GLEW_OK) {
		cout << "ERROR - glew not included" << endl;
	} 

	glutTimerFunc(33, idle, 1);    //loop animation

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glutKeyboardFunc(myKeyboard);

	vertexBuffer();

	glutMainLoop();
}
