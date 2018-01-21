#include <GL/glew.h>
#include <GL/freeglut.h>

#include <iostream>
using namespace std;

void myRender(void);

void myKeyboard(unsigned char key, int x, int y);


int main(int argc, char *argv[]) {

	glutInit(&argc, argv);
	glewInit();

	cout << "Bolun" << endl;
	cout << glGetString(GL_VERSION) << endl;

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(1024, 720);
	glutInitWindowPosition(100, 100);


	glutCreateWindow("OpenGL Window");

	glutDisplayFunc(myRender);
	glutKeyboardFunc(myKeyboard);

	if (glewInit() != GLEW_OK) {
		cout << "ERROR - glew not included" << endl;
	}

	glutMainLoop();
}

void myKeyboard(unsigned char key, int x, int y) {
	if (key == 27) {
		exit(0);
	}
}

void myRender(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float vertex[15] = {
		1, 0, 0,
		-0.5, -0.5,
		0, 1, 0,
		0.5, -0.5,
		0, 0, 1,
		0.0, 0.5
	};
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, 15 * sizeof(float), vertex, GL_STATIC_DRAW);

	//glDrawArrays(GL_TRIANGLES, 0, 3);

	glEnableVertexAttribArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (const void*)12);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//glDisableVertexAttribArray(0);

	glutSwapBuffers();
}