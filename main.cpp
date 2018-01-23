/*

	Bolun Gao
	
	*/
#include <GL/glew.h>
#include <GL/freeglut.h>

#include <iostream>
using namespace std;

float colorR;
float colorG;
float colorB;

void myRender(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glClearColor(colorR, colorG, colorB, 0.0f);

	glutSwapBuffers();
}

void myKeyboard(unsigned char key, int x, int y) {
	if (key == 27) {
		exit(0);
	}
}

void timer(int value) {

	colorR = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	colorG = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	colorB = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

	myRender();
	glutPostRedisplay();
	glutTimerFunc(1000, timer, 3);
}

int main(int argc, char *argv[]) {

	glutInit(&argc, argv);
	glewInit();

	cout << "Bolun" << endl;

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(1024, 720);
	glutInitWindowPosition(100, 100);


	glutCreateWindow("OpenGL Window");

	glutDisplayFunc(myRender);
	glutTimerFunc(1000, timer, 3);
	glutKeyboardFunc(myKeyboard);

	if (glewInit() != GLEW_OK) {
		cout << "ERROR - glew not included" << endl;
	}

	glutMainLoop();
}
