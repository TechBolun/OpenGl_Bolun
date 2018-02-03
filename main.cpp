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

GLuint numVertices;
GLuint numFaces;
GLuint numIndices;

GLuint fullTransform;

GLuint LeftMouseButtonDown;
GLuint RightMouseButtonDown;

float leftMouseX = 0.0f;
int preLeftMouseX;
int preLeftMouseY;
float leftMouseY;
float rightMouseX;
int preRightMouseX;

Point3f *position;
unsigned int *indices;

Matrix4<float> model;
Matrix4<float> view;
Matrix4<float> projection;

Matrix4<float> MVP;

Matrix4<float> temp;

GLuint VAO;
GLuint VBO;	// vertices
GLuint EAO; // indeice 

GLSLProgram shaderProgram;

TriMesh objFile;

/*
	set matrix4
*/

static void setTransformation() {


	view.SetView(Point3f(0.0f, -90.0f, 30.0f), Point3f(0.0f, 0.0f, 0.0f), Point3f(0.0f, 1.0f, 0.0f));
	projection.SetPerspective(1, 1, 0.1, 200);

	MVP = projection * view * model;
}

/*
	display function
*/

static void myRender() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shaderProgram.GetID());

	glUniformMatrix4fv(fullTransform, 1, GL_FALSE, &MVP.data[0]);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EAO);

	//glDrawArrays(GL_POINTS, 0, numVertices); // from 0, num of vertex need to draw
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);

	glutSwapBuffers();
}

/*
	load obj file
*/

static void loadObj() {

	bool loadSuccess = objFile.LoadFromFileObj("Mesh/teapot.obj");

	if (!loadSuccess) {
		cout << "load file ERROR" << endl;
	}

	model.SetIdentity();

	objFile.ComputeBoundingBox();

	model.AddTrans(-(objFile.GetBoundMax() + objFile.GetBoundMin()) / 2);

	numVertices = objFile.NV();

	position = new Point3f[numVertices];

	for (int i = 0; i < numVertices; i++) {
		position[i].x = objFile.V(i).x;
		position[i].y = objFile.V(i).y;
		position[i].z = objFile.V(i).z;
	}

	numFaces = objFile.NF();

	numIndices = numFaces * 3;

	indices = new unsigned int[numIndices];

	for (int i = 0; i < numFaces; i ++) {

		indices[3 * i] = objFile.F(i).v[0];
		indices[3 * i + 1] = objFile.F(i).v[1];
		indices[3 * i + 2] = objFile.F(i).v[2];

	}
}

static void indexBuffer() {

	glGenBuffers(1, &EAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EAO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numIndices, indices, GL_STATIC_DRAW);
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(Point3f) * numVertices, position, GL_STATIC_DRAW);

	indexBuffer();

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

	if (LeftMouseButtonDown) {

		view *= Matrix4<float>::MatrixRotationX(leftMouseX);

		view *= Matrix4<float>::MatrixRotationY(leftMouseY);

	}

	if (RightMouseButtonDown) {

		view.AddTrans(Point3f(0.0f, 0.0f, rightMouseX)) ;
	}


	MVP = projection * view * model;

	glutPostRedisplay();

}

/*
	keyboard input
*/
static void myKeyboard(unsigned char key, int x, int y) {

	switch (key) {
	case 27:
		delete[] position;
		delete[] indices;
		exit(0);
	}
}

/*
	get mouse click
*/
static void myMouse(int button, int state, int x, int y) {

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		RightMouseButtonDown = 1;
	}
	else {
		RightMouseButtonDown = 0;
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
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

		rightMouseX = 0.5f * (preRightMouseX - x);

		preRightMouseX = x;
	}

	if (LeftMouseButtonDown) 
	{
		leftMouseX = 0.05f * (preLeftMouseX - x);
		leftMouseY = 0.05f * (preLeftMouseY - y);

		preLeftMouseX = x;
		preLeftMouseY = y;
	}


	
	cout << "Mouse---xxxxxxxxxx" << leftMouseX  << endl;
	cout << "Mouse---yyyyyyyyyy" << leftMouseY << endl;



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

	//glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	vertexBuffer();

	compileShader();

	setTransformation();

	glutMainLoop();
}
