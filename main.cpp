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

GLuint MVP_ID_RenderToTexture;

GLuint MVP_ID;
GLuint MV_ID;
GLuint MW_ID;
GLuint ambientLightUniformLocation;
GLuint lightPositionUniformLocation;
GLuint cameraPositionUniformLocation;
GLuint diffuseUniformLocation;
GLuint specularUniformLocation;

GLuint MVP_ID_Skybox;
GLuint MV_ID_Skybox;
GLuint MW_ID_Skybox;

GLuint diffuse_ID;
GLuint specular_ID;
GLuint skyboxTexture_ID;

bool LeftMouseButtonDown;
bool RightMouseButtonDown;
bool controlButtonDown;
bool altButtonDown;

float leftMouseRotationX;
float leftMouseRotationY;
int preLeftMouseX;
int preLeftMouseY;
float rightMouseScale;
float rightMouseY;
int preRightMouseX;
int preRightMouseY;

const int window_width = 800;
const int window_height = 800;

unsigned width, height;
vector<unsigned char> texture;
unsigned int convertTextureToData;

Point3f *position;
Point3f *normal;
Point3f *uv;
unsigned int *indices;

Matrix4f model;
Matrix4f view;
Matrix4f projection;

Matrix4f model_Plane;
Matrix4f view_Plane;
Matrix4f projection_Plane;

Matrix4f model_skybox;

Matrix4f MVP;
Matrix4f MV;;
Matrix4f temp;

Matrix4f MVP_plane;

Matrix4f MVP_skybox;

GLuint VAO;
GLuint VBO;	// vertices
GLuint VNBO;	// vertices  NORMAL
GLuint UVBO; // UV
GLuint EAO; // indeice 
GLuint FBO;

GLuint plane_VAO;
GLuint plane_VBO;

GLuint skybox_VAO;
GLuint skybox_VBO;

GLSLProgram shaderProgram;
GLSLProgram shaderProgram_RenderToTexture;
GLSLProgram shaderProgram_Skybox;

TriMesh::Mtl mat;

TriMesh objFile;

Point3f cameraPosition = Point3f(0.0f, -90.0f, 30.0f);
Point3f targetPosition = Point3f(0.0f, 0.0f, 0.0f);

Point3f lightPosition = Point3f(0.0f, 0.0f, 10.0f);

Point3f diffuseColor = Point3f(1.0f, 0.0f, 1.0f);
Point3f specularColor = Point3f(0.0f, 1.0f, 1.0f);
Point3f ambientLight = Point3f(0.2f, 0.2f, 0.2f);

GLRenderTexture2D renderedTexture;

string teapotPath = "Mesh/teapot.obj";

GLfloat plane_vextex_buffer_data[] = {

	-15.0f, -15.0f, 0.0f,  //vertex
	0.0f, 0.0f, 0.0f,   //normal
	0.0,  0.0, 0.0f,	//UV

	15.0f, -15.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	1.0,  0.0, 0.0f,

	-15.0f,  15.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0,  1.0, 0.0f,

	-15.0f,  15.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0,  1.0, 0.0f,

	15.0f, -15.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	1.0,  0.0, 0.0f,

	15.0f,  15.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	1.0,  1.0, 0.0f,
};

GLfloat skybox_vextex_buffer_data[] = {

	-20.0f,  20.0f, -20.0f,
	-20.0f, -20.0f, -20.0f,
	20.0f, -20.0f, -20.0f,
	20.0f, -20.0f, -20.0f,
	20.0f,  20.0f, -20.0f,
	-20.0f,  20.0f, -20.0f,

	-20.0f, -20.0f,  20.0f,
	-20.0f, -20.0f, -20.0f,
	-20.0f,  20.0f, -20.0f,
	-20.0f,  20.0f, -20.0f,
	-20.0f,  20.0f,  20.0f,
	-20.0f, -20.0f,  20.0f,

	20.0f, -20.0f, -20.0f,
	20.0f, -20.0f,  20.0f,
	20.0f,  20.0f,  20.0f,
	20.0f,  20.0f,  20.0f,
	20.0f,  20.0f, -20.0f,
	20.0f, -20.0f, -20.0f,

	-20.0f, -20.0f,  20.0f,
	-20.0f,  20.0f,  20.0f,
	20.0f,  20.0f,  20.0f,
	20.0f,  20.0f,  20.0f,
	20.0f, -20.0f,  20.0f,
	-20.0f, -20.0f,  20.0f,

	-20.0f,  20.0f, -20.0f,
	20.0f,  20.0f, -20.0f,
	20.0f,  20.0f,  20.0f,
	20.0f,  20.0f,  20.0f,
	-20.0f,  20.0f,  20.0f,
	-20.0f,  20.0f, -20.0f,

	-20.0f, -20.0f, -20.0f,
	-20.0f, -20.0f,  20.0f,
	20.0f, -20.0f, -20.0f,
	20.0f, -20.0f, -20.0f,
	-20.0f, -20.0f,  20.0f,
	20.0f, -20.0f,  20.0f
};

vector<string> skyboxFaces = {
	"Skybox/cubemap_negx.PNG",
	"Skybox/cubemap_negy.PNG",
	"Skybox/cubemap_negz.PNG",
	"Skybox/cubemap_posx.PNG",
	"Skybox/cubemap_posy.PNG",
	"Skybox/cubemap_posz.PNG",
};

void fullTransformation();
void modelToViewTransformation();
void BlinnShading();
void renderToTexture();
void indexBuffer();
void compileShader();
void initialTexture();
void renderToTexture();
void initialShaderProgram();

/*
display function
*/

void myRender() {

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	renderedTexture.Bind();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shaderProgram.GetID());

	BlinnShading();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuse_ID);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, specular_ID);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, numIndices);
	glBindVertexArray(0);

	renderedTexture.Unbind();


	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shaderProgram_RenderToTexture.GetID());

	MVP_ID_RenderToTexture = glGetUniformLocation(shaderProgram_RenderToTexture.GetID(), "MVP_RenderToTexturePlane");

	glUniformMatrix4fv(MVP_ID_RenderToTexture, 1, GL_FALSE, &MVP_plane.data[0]);

	glActiveTexture(GL_TEXTURE0);
	renderedTexture.BindTexture();

	glBindVertexArray(plane_VAO);
	glDrawArrays(GL_TRIANGLES, 0, sizeof(plane_vextex_buffer_data));
	glBindVertexArray(0);

	glutSwapBuffers();
}

/*
set model to projection transformation matrix4
*/
void fullTransformation() {

	objFile.ComputeBoundingBox();

	model.SetIdentity();

	model.AddTrans(-(objFile.GetBoundMax() + objFile.GetBoundMin()) / 2);

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

void skyboxTransformation() {

	model_skybox.SetIdentity();
	model_skybox.SetScale(Point3f(5, 5, 5));

	MVP_skybox = projection * view * model_skybox;

	MVP_ID_Skybox = glGetUniformLocation(shaderProgram_Skybox.GetID(), "MVP_skybox");

	glUniformMatrix4fv(MVP_ID_Skybox, 1, GL_FALSE, &MVP_skybox.data[0]);

}

void planeTransformation() {

	model_Plane.SetIdentity();
	view_Plane = view;
	projection_Plane = projection;

	MVP_plane = projection_Plane * view_Plane * model_Plane;

}

void BlinnShading() {

	glUniformMatrix4fv(MVP_ID, 1, GL_FALSE, &MVP.data[0]);
	glUniformMatrix4fv(MV_ID, 1, GL_FALSE, &MV.data[0]);
	glUniformMatrix4fv(MW_ID, 1, GL_FALSE, &model.data[0]);

	glUniform3fv(lightPositionUniformLocation, 1, &lightPosition[0]);

	glUniform3fv(cameraPositionUniformLocation, 1, &cameraPosition[0]);
	glUniform3fv(ambientLightUniformLocation, 1, &ambientLight[0]);

	glUniform1i(diffuseUniformLocation, 0);
	glUniform1i(specularUniformLocation, 1);

}

void initialShaderProgram() {

	shaderProgram.CreateProgram();
	shaderProgram.BuildFiles("vertex.glsl", "fragment.glsl");

	shaderProgram_RenderToTexture.CreateProgram();
	shaderProgram_RenderToTexture.BuildFiles("renderToTextureVertex.glsl", "renderToTextureFragment.glsl");

	shaderProgram_Skybox.CreateProgram();
	shaderProgram_Skybox.BuildFiles("SkyboxVertex.glsl", "SkyboxFragment.glsl");
}

/*
load obj file
*/

void loadObj(string name) {

	bool loadSuccess = objFile.LoadFromFileObj(name.c_str());


	if (!loadSuccess) {
		cout << "load file ERROR" << endl;
	}

	numVertices = objFile.NV(); // get vertices

	numVertexNormal = objFile.NVN(); //get vertex normal


	numFaces = objFile.NF();

	numIndices = numFaces * 3;


	indices = new unsigned int[numIndices];


	position = new Point3f[numIndices];

	normal = new Point3f[numIndices];

	uv = new Point3f[numIndices];

	for (int i = 0; i < numFaces; i++) {

		indices[3 * i] = objFile.F(i).v[0];
		indices[3 * i + 1] = objFile.F(i).v[1];
		indices[3 * i + 2] = objFile.F(i).v[2];

		TriMesh::TriFace vertexFace = objFile.F(i);

		// numfaces * 3 = positions
		position[3 * i] = objFile.V(vertexFace.v[0]);
		position[3 * i + 1] = objFile.V(vertexFace.v[1]);
		position[3 * i + 2] = objFile.V(vertexFace.v[2]);

		TriMesh::TriFace normalFace = objFile.FN(i);

		normal[3 * i] = objFile.VN(normalFace.v[0]);
		normal[3 * i + 1] = objFile.VN(normalFace.v[1]);
		normal[3 * i + 2] = objFile.VN(normalFace.v[2]);

		TriMesh::TriFace texFace = objFile.FT(i);

		uv[3 * i] = Point3f(objFile.VT(texFace.v[0]));
		uv[3 * i + 1] = Point3f(objFile.VT(texFace.v[1]));
		uv[3 * i + 2] = Point3f(objFile.VT(texFace.v[2]));

	}
}

void initialTexture() {

	mat = objFile.M(0); //get first material

	{

		unsigned width, height;
		vector<unsigned char> texture;

		// store diffuse to texture

		convertTextureToData = decode(texture, width, height, mat.map_Kd.data, LCT_RGBA); //Converts PNG file from disk to raw pixel data in memory.

		glGenTextures(1, &diffuse_ID);
		glBindTexture(GL_TEXTURE_2D, diffuse_ID);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.data());

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glGenerateMipmap(GL_TEXTURE_2D);

	}

	{
		unsigned width, height;
		vector<unsigned char> texture;

		// store specular to texture

		convertTextureToData = decode(texture, width, height, mat.map_Ks.data, LCT_RGBA); //Converts PNG file from disk to raw pixel data in memory.

		glGenTextures(1, &specular_ID);
		glBindTexture(GL_TEXTURE_2D, specular_ID);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.data());

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glGenerateMipmap(GL_TEXTURE_2D);
	}
}


void initialSkybox() {

	glGenVertexArrays(1, &skybox_VAO);
	glBindVertexArray(skybox_VAO);

	glGenBuffers(1, &skybox_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, skybox_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skybox_vextex_buffer_data), &skybox_vextex_buffer_data[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Point3f), 0);

}

void loadSkybox() {

	glGenTextures(1, &skyboxTexture_ID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture_ID);

	unsigned width, height;
	vector<unsigned char> skyboxTexture;

	for (int i = 0; i < skyboxFaces.size(); i++) {

		convertTextureToData = decode(skyboxTexture, width, height, skyboxFaces[i], LCT_RGBA);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, skyboxTexture.data());

	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

}

void renderToTexture() {

	renderedTexture.Initialize(true, 3, window_width, window_height);

	renderedTexture.SetTextureMaxAnisotropy();
	renderedTexture.SetTextureFilteringMode(GL_LINEAR, 0);
	renderedTexture.BuildTextureMipmaps();

	glGenVertexArrays(1, &plane_VAO);
	glBindVertexArray(plane_VAO);

	glGenBuffers(1, &plane_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, plane_VBO);
	glBufferData(GL_ARRAY_BUFFER, (sizeof(plane_vextex_buffer_data)), &plane_vextex_buffer_data[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(Point3f), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(Point3f), (void*)(sizeof(Point3f)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(Point3f), (void*)(2 * sizeof(Point3f)));

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

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// vertex buffer
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Point3f) * numIndices, position, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);


	//vertex normal
	glGenBuffers(1, &VNBO);
	glBindBuffer(GL_ARRAY_BUFFER, VNBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Point3f) * numIndices, normal, GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//texCoord
	glGenBuffers(1, &UVBO);
	glBindBuffer(GL_ARRAY_BUFFER, UVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Point3f) * numIndices, uv, GL_STATIC_DRAW);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//indexBuffer();
}


/*
compile shaders
*/
void compileShader() {


	MVP_ID = glGetUniformLocation(shaderProgram.GetID(), "MVP_tranform");
	MV_ID = glGetUniformLocation(shaderProgram.GetID(), "MV_tranform");
	MW_ID = glGetUniformLocation(shaderProgram.GetID(), "MW_tranform");

	lightPositionUniformLocation = glGetUniformLocation(shaderProgram.GetID(), "lightPosition_Bolun");

	ambientLightUniformLocation = glGetUniformLocation(shaderProgram.GetID(), "ambientColor_Bolun");
	cameraPositionUniformLocation = glGetUniformLocation(shaderProgram.GetID(), "cameraPosition_Bolun");

	diffuseUniformLocation = glGetUniformLocation(shaderProgram.GetID(), "diffuse_Bolun");
	specularUniformLocation = glGetUniformLocation(shaderProgram.GetID(), "specular_Bolun");

	assert(MVP_ID != 0xFFFFFFFF);
}

void myIdle() {

	if (controlButtonDown) {

		if (LeftMouseButtonDown) {

			lightPosition = Matrix3f::MatrixRotationY(leftMouseRotationX) * lightPosition;

		}
	}
	else if (altButtonDown) {

		if (LeftMouseButtonDown) {

			view_Plane *= Matrix4f::MatrixRotationX(leftMouseRotationX);

			view_Plane *= Matrix4f::MatrixRotationY(leftMouseRotationY);

			cout << 1 << endl;
		}
		else if (RightMouseButtonDown) {

			view_Plane.AddTrans(Point3f(0.0f, 0.0f, rightMouseScale));

		}

		//MVP_plane = projectionPlane * viewPlane * modelPlane;
	}
	else if (LeftMouseButtonDown) {

		view *= Matrix4f::MatrixRotationX(leftMouseRotationX);

		view *= Matrix4f::MatrixRotationY(leftMouseRotationY);

		//MVP = projection * view * model;
	}
	else if (RightMouseButtonDown) {

		view.AddTrans(Point3f(0.0f, 0.0f, rightMouseScale));

		//MVP = projection * view * model;
	}

	MVP_plane = projection_Plane * view_Plane * model_Plane;

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
		delete[] uv;
		delete[] indices;
		exit(0);

	}
}

void functionKeyDown(int key, int x, int y) {

	switch (key)
	{
	case 0x72:
		controlButtonDown = true;
		break;

	case 0x73:
		controlButtonDown = true;
		break;

	case 116:
		altButtonDown = true;
		break;

	default:
		break;
	}

}

void functionKeyUp(int key, int x, int y) {

	switch (key)
	{
	case 0x72:
		controlButtonDown = false;
		break;

	case 0x73:
		controlButtonDown = false;
		break;

	case 116:
		altButtonDown = false;
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
	glutInitWindowSize(window_width, window_height);
	glutInitWindowPosition(100, 100);

	glutCreateWindow("OpenGL Window - Bolun Gao");

	if (glewInit() != GLEW_OK) {
		cout << "ERROR - glew not included" << endl;
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	initialShaderProgram();
	loadObj(teapotPath);
	initialTexture();
	renderToTexture();
	vertexBuffer();
	compileShader();

	fullTransformation();
	modelToViewTransformation();
	planeTransformation();
	skyboxTransformation();

	glutDisplayFunc(myRender);
	glutIdleFunc(myIdle);

	glutKeyboardFunc(myKeyboard);
	glutSpecialFunc(functionKeyDown);
	glutSpecialUpFunc(functionKeyUp);
	glutMouseFunc(myMouse);
	glutMotionFunc(myMouseMotion);

	glutMainLoop();
}
