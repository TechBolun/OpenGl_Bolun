/*
	Bolun Gao

	*/
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "cyMatrix.h"
#include "cyTriMesh.h"
#include "cyGL.h"
#include "cyPoint.h"
#include <iostream>
using namespace std;
using namespace cy;

GLuint numVertex;

GLuint fullTransform;

GLuint LeftMouseButtonDown;
GLuint RightMouseButtonDown;
GLuint leftMouseX;
GLuint oldleftMouseX;
GLuint oldleftMouseY;
GLuint leftMouseY;

Point3f *position;

Matrix4<float> model;
Matrix4<float> view;
Matrix4<float> projection;

Matrix4<float> MVP;

GLuint VAO;
GLuint VBO;

GLSLProgram shaderProgram;

/*
	set matrix4
*/

static void setTransformation() {

	model.SetIdentity();
	view.SetView(Point3f(0.0f, -90.0f, 30.0f), Point3f(0.0f, 0.0f, 0.0f), Point3f(0.0f, 1.0f, 0.0f));
	projection.SetPerspective(1, 1, 0.1, 100);

	MVP = projection * view * model;

	glUseProgram(shaderProgram.GetID());

	glUniformMatrix4fv(fullTransform, 1, GL_FALSE, &MVP.data[0]);
}

/*
	display function
*/

static void myRender() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	setTransformation();

	glEnableVertexAttribArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_POINTS, 0, numVertex); // from 0, num of vertex need to draw

	glDisableVertexAttribArray(0);

	glutSwapBuffers();
}

/*
	load obj file
*/

static void loadObj() {

	TriMesh objFile;
	bool loadSuccess = objFile.LoadFromFileObj("Mesh/teapot.obj");

	if (!loadSuccess) {
		cout << "load file ERROR" << endl;
	}

	numVertex = objFile.NV();

	position = new Point3f[numVertex];

	for (int i = 0; i < numVertex; i++) {
		position[i].x = objFile.V(i).x;
		position[i].y = objFile.V(i).y;
		position[i].z = objFile.V(i).z;
	}
}

/*
	create vertex buffer
*/
static void vertexBuffer() {

	loadObj();

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Point3f) * numVertex, position, GL_STATIC_DRAW);

}

/*
	compile shaders
*/
static void compileShader() {

	shaderProgram.CreateProgram();
	shaderProgram.BuildFiles("vertex.glsl", "fragment.glsl");

	glUseProgram(shaderProgram.GetID());

	fullTransform = glGetUniformLocation(shaderProgram.GetID(), "MVP_tranform");
	assert(fullTransform != 0xFFFFFFFF);
}

static void myIdle() {

	//view.SetRotationXYZ(leftMouseX * 10, leftMouseY * 10, 0);


	glutPostRedisplay();

}

/*
	keyboard input
*/
static void myKeyboard(unsigned char key, int x, int y) {
	if (key == 27) {
		delete[] position;
		exit(0);
	}
}

/*
	get mouse click
*/
static void myMouse(int button, int state, int x, int y) {

	if (button == GLUT_RIGHT_BUTTON) {
		RightMouseButtonDown = 1;
	}
	else {
		RightMouseButtonDown = 0;
	}
	if (button == GLUT_LEFT_BUTTON) {
		LeftMouseButtonDown = 1;
	}
	else {
		LeftMouseButtonDown = 0;
	}
}

/*
	tracking mouse movement
*/

static void myMouseMotion(int x, int y) {

	if (RightMouseButtonDown) {

		
	}
	if (LeftMouseButtonDown) {


		leftMouseX = x * 0.01;
		cout << "Mouse---xxxxxxxxxx" << leftMouseX << endl;
		leftMouseY = y * 0.01;
		cout << "Mouse---yyyyyyyyyy" << leftMouseY << endl;

		
	}

}

int main(int argc, char *argv[]) {

	glutInit(&argc, argv);

	cout << "Bolun" << endl;

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(1024, 720);
	glutInitWindowPosition(100, 100);

	glutCreateWindow("OpenGL Window - Bolun Gao");

	if (glewInit() != GLEW_OK) {
		cout << "ERROR - glew not included" << endl;
	}

	glutDisplayFunc(myRender);
	glutIdleFunc(myIdle);

	glutKeyboardFunc(myKeyboard);
	glutMouseFunc(myMouse);
	glutMotionFunc(myMouseMotion);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	vertexBuffer();

	compileShader();

	glutMainLoop();
}
