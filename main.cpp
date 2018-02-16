/*
	Bolun Gao

	*/
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "lodepng/lodepng.h"
#include "cyCodeBase/cyMatrix.h"
#include "cyCodeBase/cyTriMesh.h"
#include "cyCodeBase/cyGL.h"
#include "cyCodeBase/cyPoint.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <stdio.h>

using namespace std;
using namespace cy;
using namespace lodepng;

GLuint numVertices;
GLuint numFaces;
GLuint numIndices;
GLuint numNormalFaces;
GLuint numVertexNormal;
GLuint numUVindices;

GLuint fullTransform;
GLuint modelToViewTransform;
GLuint modelToWorldTransform;
GLuint ambientLightUniformLocation;
GLuint lightPositionUniformLocation;
GLuint cameraPositionUniformLocation;
GLuint diffuseUniformLocation;
GLuint specularUniformLocation;

GLuint diffuse_ID;
GLuint specular_ID;

bool LeftMouseButtonDown;
bool RightMouseButtonDown;
bool controlButtonDown;

float leftMouseRotationX;
float leftMouseRotationY;
int preLeftMouseX;
int preLeftMouseY;
float rightMouseScale;
float rightMouseY;
int preRightMouseX;
int preRightMouseY;

//unsigned width, height;
//vector<unsigned char> texture;
unsigned int convertPNG;

Point3f *position;
Point3f *normal;
Point2f *uv;
unsigned int *indices;
unsigned int *UVindices;

Point3f *posIndices;

Matrix4<float> model;
Matrix4<float> view;
Matrix4<float> projection;

Matrix4<float> MVP;
Matrix4f MV;;
Matrix4<float> temp;

GLuint VAO;
GLuint VBO;	// vertices
GLuint VNBO;	// vertices  NORMAL
GLuint UVBO; // UV
GLuint EAO; // indeice 

GLSLProgram shaderProgram;

TriMesh::Mtl mat;

TriMesh objFile;

Point3f cameraPosition = Point3f(0.0f, -90.0f, 30.0f);
Point3f targetPosition = Point3f(0.0f, 0.0f, 0.0f);

Point3f lightPosition = Point3f(0.0f, 0.0f, 10.0f);

Point3f diffuseColor = Point3f(1.0f, 0.0f, 1.0f);
Point3f specularColor = Point3f(0.0f, 1.0f, 1.0f);
Point3f ambientLight = Point3f(0.2f, 0.2f, 0.2f);


/*
	set model to projection transformation matrix4
*/
static void fullTransformation() {


	view.SetView(cameraPosition, targetPosition, Point3f(0.0f, 1.0f, 0.0f));
	projection.SetPerspective(1, 1, 0.1, 300);

	MVP = projection * view * model;
}

/*
	set model to view transformation matrix4
*/
void modelToViewTransformation() {

	MV = view * model;
	MV.Invert();
	MV.Transpose();
}

///*
//	set light tranformation 
//*/
//void lightTransformation() {
//	lightPosition = Point3f();
//	//lightPosition = Matrix3f::MatrixRotationX()
//}

void BlinnShading() {

	glUniform3fv(lightPositionUniformLocation, 1, &lightPosition[0]);
	glUniform3fv(cameraPositionUniformLocation, 1, &cameraPosition[0]);
	glUniform3fv(ambientLightUniformLocation, 1, &ambientLight[0]);

	glUniform3fv(lightPositionUniformLocation, 1, &lightPosition[0]);

	glUniform1i(diffuseUniformLocation, 0);
	glUniform1i(specularUniformLocation, 1);
	//glUniform3fv(diffuseUniformLocation, 1, &diffuseColor[0]);
	//glUniform3fv(specularUniformLocation, 1, &specularColor[0]);

}

void initialTexture() {






}

/*
	display function
*/

void myRender() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shaderProgram.GetID());

	glUniformMatrix4fv(fullTransform, 1, GL_FALSE, &MVP.data[0]);
	glUniformMatrix4fv(modelToViewTransform, 1, GL_FALSE, &MV.data[0]);
	glUniformMatrix4fv(modelToWorldTransform, 1, GL_FALSE, &model.data[0]);

	BlinnShading();

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, VNBO);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, UVBO);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EAO);

	//glDrawArrays(GL_TRIANGLES, 0, numIndices);

	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glutSwapBuffers();
}

/*
	load obj file
*/

void loadObj() {

	bool loadSuccess = objFile.LoadFromFileObj("teapot.obj");


	if (!loadSuccess) {
		cout << "load file ERROR" << endl;
	}

	model.SetIdentity();

	objFile.ComputeBoundingBox();

	model.AddTrans(-(objFile.GetBoundMax() + objFile.GetBoundMin()) / 2);

	numVertices = objFile.NV(); // get vertices
	cout << numVertices << endl;

	numVertexNormal = objFile.NVN(); //get vertex normal
	cout << numVertexNormal << endl;

	//store vertices to position buffer

	position = new Point3f[numVertices];

	for (int i = 0; i < numVertices; i++) {
		position[i].x = objFile.V(i).x;
		position[i].y = objFile.V(i).y;
		position[i].z = objFile.V(i).z;
	}

	// store vertex normal to normal buffer

	normal = new Point3f[numVertexNormal];

	for (int i = 0; i < numVertexNormal; i++) {
		normal[i].x = objFile.VN(i).x;
		normal[i].y = objFile.VN(i).y;
		normal[i].z = objFile.VN(i).z;
	}

	// store face vertices to indices buffer

	numFaces = objFile.NF();

	numIndices = numFaces * 3;

	indices = new unsigned int[numIndices];


	

	numUVindices = objFile.NVT();

	cout << numUVindices << endl;

	uv = new Point2f[numIndices];

	UVindices = new unsigned int[numUVindices];

	for (int i = 0; i < numUVindices; i++) {
		uv[i].x = objFile.V(i).x;
		uv[i].y = objFile.V(i).y;
	}


	posIndices = new Point3f[numIndices];


	for (int i = 0; i < numFaces; i ++) {

		indices[3 * i] = objFile.F(i).v[0];
		indices[3 * i + 1] = objFile.F(i).v[1];
		indices[3 * i + 2] = objFile.F(i).v[2];

		posIndices[3 * i] = objFile.V(objFile.F(i).v[0]);
		posIndices[3 * i + 1] = objFile.V(objFile.F(i).v[1]);
		posIndices[3 * i + 2] = objFile.V(objFile.F(i).v[2]);

		TriMesh::TriFace texFace = objFile.FT(i);

		uv[3 * i] = Point2f(objFile.VT(texFace.v[0]));
		uv[3 * i + 1] = Point2f(objFile.VT(texFace.v[1]));
		uv[3 * i + 2] = Point2f(objFile.VT(texFace.v[2]));

	}

	for (int i = 0; i < numFaces; i++) {

	}

	mat = objFile.M(0); //get first material

	
	{

		unsigned width, height;
		vector<unsigned char> texture;

		// store diffuse to texture

		convertPNG = decode(texture, width, height, mat.map_Kd.data, LCT_RGBA); //Converts PNG file from disk to raw pixel data in memory.

		glActiveTexture(GL_TEXTURE0);
		glGenTextures(1, &diffuse_ID);
		glBindTexture(GL_TEXTURE_2D, diffuse_ID);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.data());

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	}

	{
		unsigned width, height;
		vector<unsigned char> texture;

		// store specular to texture

		convertPNG = decode(texture, width, height, mat.map_Ks.data, LCT_RGBA); //Converts PNG file from disk to raw pixel data in memory.

		glActiveTexture(GL_TEXTURE1);
		glGenTextures(1, &specular_ID);
		glBindTexture(GL_TEXTURE_2D, specular_ID);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.data());

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}



	
}

void indexBuffer() {

	glGenBuffers(1, &EAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EAO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numIndices, indices, GL_STATIC_DRAW);
}

/*
	create vertex buffer
*/
void vertexBuffer() {

	loadObj();

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// vertex buffer
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Point3f) * numVertices, position, GL_STATIC_DRAW);

	//vertex normal
	glGenBuffers(1, &VNBO);
	glBindBuffer(GL_ARRAY_BUFFER, VNBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Point3f) * numVertexNormal, normal, GL_STATIC_DRAW);

	//vertex normal
	glGenBuffers(1, &UVBO);
	glBindBuffer(GL_ARRAY_BUFFER, UVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Point2f) * numIndices, uv, GL_STATIC_DRAW);

	indexBuffer();
}


/*
	compile shaders
*/
void compileShader() {

	shaderProgram.CreateProgram();
	shaderProgram.BuildFiles("vertex.glsl", "fragment.glsl");

	glUseProgram(shaderProgram.GetID());

	fullTransform = glGetUniformLocation(shaderProgram.GetID(), "MVP_tranform");
	modelToViewTransform = glGetUniformLocation(shaderProgram.GetID(), "MV_tranform");
	modelToWorldTransform = glGetUniformLocation(shaderProgram.GetID(), "MW_tranform");

	lightPositionUniformLocation = glGetUniformLocation(shaderProgram.GetID(), "lightPosition_transform");

	ambientLightUniformLocation = glGetUniformLocation(shaderProgram.GetID(), "ambientColor_Bolun");
	cameraPositionUniformLocation = glGetUniformLocation(shaderProgram.GetID(), "cameraPosition_Bolun");

	diffuseUniformLocation = glGetUniformLocation(shaderProgram.GetID(), "diffuse_Bolun");
	specularUniformLocation = glGetUniformLocation(shaderProgram.GetID(), "specular_Bolun");

	assert(fullTransform != 0xFFFFFFFF);
}

void myIdle() {

	if (LeftMouseButtonDown) {

		view *= Matrix4f::MatrixRotationX(leftMouseRotationX);

		view *= Matrix4f::MatrixRotationY(leftMouseRotationY);
	}
	else if (RightMouseButtonDown) {

		view.AddTrans(Point3f(0.0f, 0.0f, rightMouseScale)) ;
	}
	else if (controlButtonDown) {

		lightPosition = Matrix3f::MatrixRotationZ(0.1f * leftMouseRotationX) * lightPosition;
	}

	MVP = projection * view * model;

	glutPostRedisplay();

}

/*
	keyboard input
*/
void myKeyboard(unsigned char key, int x, int y) {

	switch (key) {
	case 27:
		delete[] position;
		delete[] normal;
		delete[] indices;
		delete[] uv;
		delete[] UVindices;
		exit(0);

	}
}

void functionKeyDown(GLint key, GLint x, GLint y) {

	switch (key)
	{
		case 0x72:
			controlButtonDown = true;
			break;

		case 0x73:
			controlButtonDown = true;
			break;

		default:
			break;
	}

}

void functionKeyUp(GLint key, GLint x, GLint y) {

	switch (key)
	{
	case 0x72:
		controlButtonDown = false;
		break;

	case 0x73:
		controlButtonDown = false;
		break;

	default:
		break;
	}

}

/*
	get mouse click
*/
static void myMouse(int button, int state, int x, int y) {

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		RightMouseButtonDown = true;
	}
	else {
		RightMouseButtonDown = false;
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		LeftMouseButtonDown = true;
	}
	else {
		LeftMouseButtonDown = false;
	}

	preRightMouseX = x;
	preRightMouseY = y;

	preLeftMouseX = x;
	preLeftMouseY = y;
}

/*
	tracking mouse movement
*/

static void myMouseMotion(int x, int y) {

	if (RightMouseButtonDown) {


			rightMouseScale = 0.5f * (preRightMouseX - x);
			rightMouseScale = 0.5f * (preRightMouseY - y);

			preRightMouseX = x;
			preRightMouseY = y;

	}

	if (LeftMouseButtonDown) {


			leftMouseRotationX = 0.02f * (preLeftMouseX - x);
			leftMouseRotationY = 0.02f * (preLeftMouseY - y);

			preLeftMouseX = x;
			preLeftMouseY = y;

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

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	
	glutDisplayFunc(myRender);
	glutIdleFunc(myIdle);

	glutKeyboardFunc(myKeyboard);
	glutSpecialFunc(functionKeyDown);
	//glutSpecialFunc(functionKeyUp);
	glutMouseFunc(myMouse);
	glutMotionFunc(myMouseMotion);

	initialTexture();

	vertexBuffer();

	compileShader();

	fullTransformation();
	modelToViewTransformation();

	glutMainLoop();
}
