/*
 * main.cpp
 *
 *  Created on: Dec 2, 2011
 *      Author: kamiyo
 */

#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>

#include <fstream>
#include <iostream>
#include <cstring>
#include <memory>
#include "RayTra.h"
#include "log.h"
#include "glfunc.h"


using namespace std;


//OpenGL stuff

GLFWwindow* glWindow;



// global vars
		  unsigned int*	pixels;
		  double ratio;
				 RayTra	tracer;
Imf::Array2D<Imf::Rgba> o;
Eigen::Matrix4f camera;
vector<vector<float> > vertices;
vector<float> photons;
vector<float> photonColors;
int level;
vector<GLuint> vbo;
GLuint photonVbo;
GLuint colorVbo;
string input;
Vector3d point;
int current;
int numPhotons;
int flag;

//define to convert floating point intensity to 0-255 with adjustment
//#define toInt(x) ((int)(pow(clamp(x, 0.f, 1.f), 1.f / 2.2f) * 255.f + .5f))

// OpenEXR writing
void writeRgba (const char fileName[], const Imf::Rgba *pixels, int width, int height)
{
	Imf::RgbaOutputFile file (fileName, width, height, Imf::WRITE_RGBA);
	file.setFrameBuffer (pixels, 1, width);
	file.writePixels (height);
}

//void keylistener (unsigned char key, int x, int y) {
//	if (key == 27) exit(0);
//
//}
//
//void display() {
//
//	if (!rendered) {
//		rendered = true;
//		render();
//	}
//}
//
//void idle() {
//
//}

void updateCamera() {

	float zNear = 0.01, zFar = 500;
	float d = (float) tracer.camera->_d;
	float imageRight = (float) tracer.camera->_width / (2. * d);
	float imageTop = (float) tracer.camera->_height / (2. * d);
	//camera = perspective(imageRight * zNear, zNear * imageTop, zNear, zFar).cast<float>();
	//camera *= lookat(tracer.camera->_e, tracer.camera->_dir, Vector3d(0, 1, 0)).cast<float>();
	//cout << tracer.camera->_e << " " << tracer.camera->_dir << endl;
	camera = perspective(imageRight * zNear, imageTop * zNear, zNear, zFar).cast<float>();
	camera *= lookat(tracer.camera->_e, -tracer.camera->_dir, Vector3d(0, 1, 0)).cast<float>();
}


void bufferSize_cb(GLFWwindow* glWindow, int width, int height) {
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	updateCamera();
}

void error_cb(int error, const char* description) {
	cout << "error " << error << ": " << description << endl;
}

void printOGL() {
	cout << "\r" << point << " numPhotons: " << numPhotons << "            ";
}

static void key_cb(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
		return;
	}
	if (key >= GLFW_KEY_0 && key <= GLFW_KEY_9 && action == GLFW_PRESS && mods == GLFW_MOD_SHIFT) {
		level = key - 48;
		vertices.clear();
		tracer.renderBoundingBoxes(vertices, level);
		glDeleteBuffers(vbo.size(), &vbo[0]);
		vbo.resize(vertices.size());
		glGenBuffers(vertices.size(), &vbo[0]);
		for (size_t i = 0; i < vertices.size(); i++) {
			glBindBuffer(GL_ARRAY_BUFFER, vbo[i]);
			glBufferData(GL_ARRAY_BUFFER, vertices[i].size() * sizeof(float), &vertices[i][0], GL_STATIC_DRAW);
		}
		return;
	}
	if ((key == GLFW_KEY_Q || key == GLFW_KEY_W || key == GLFW_KEY_E || key == GLFW_KEY_R) && action == GLFW_PRESS && mods != GLFW_MOD_SHIFT) {
		flag = (key == GLFW_KEY_Q) ? 0 : ((key == GLFW_KEY_W) ? 1 : ((key == GLFW_KEY_E) ? 2 : 3));
	}
	if ((key == GLFW_KEY_Q || key == GLFW_KEY_W || key == GLFW_KEY_E || key == GLFW_KEY_R) && action == GLFW_PRESS && mods == GLFW_MOD_SHIFT) {
		photons.clear(); photonColors.clear();
		flag = (key == GLFW_KEY_Q) ? 0 : ((key == GLFW_KEY_W) ? 1 : ((key == GLFW_KEY_E) ? 2 : 3));
		tracer.renderPhotonMapOGL(photons, photonColors, flag);
		glBindBuffer(GL_ARRAY_BUFFER, photonVbo);
		glBufferData(GL_ARRAY_BUFFER, photons.size() * sizeof(float), &photons[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
		glBufferData(GL_ARRAY_BUFFER, photonColors.size() * sizeof(float), &photonColors[0], GL_STATIC_DRAW);
		return;
	}
	if (((key >= GLFW_KEY_0 && key <= GLFW_KEY_9) || key == GLFW_KEY_PERIOD || key == GLFW_KEY_MINUS ) && action == GLFW_PRESS && mods != GLFW_MOD_SHIFT) {
		char inChar = key;
		input += inChar;
		stringstream ss(input);
		ss >> point[current];
		printOGL();
		return;
	}
	if (key == GLFW_KEY_TAB && action == GLFW_PRESS) {
		stringstream ss(input);
		ss >> point[current];
		current++;
		current = current % 3;
		input = "";
		printOGL();
		return;
	}
	if (key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS) {
		input = input.substr(0, input.size() - 1);
		stringstream ss(input);
		ss >> point[current];
		printOGL();
		return;
	}
	if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
		photons.clear(); photonColors.clear();
		tracer.renderClosestPhotonsOGL(photons, photonColors, point, numPhotons, flag);
		glBindBuffer(GL_ARRAY_BUFFER, photonVbo);
		glBufferData(GL_ARRAY_BUFFER, photons.size() * sizeof(float), &photons[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
		glBufferData(GL_ARRAY_BUFFER, photonColors.size() * sizeof(float), &photonColors[0], GL_STATIC_DRAW);
	}
	if (key == GLFW_KEY_LEFT_BRACKET && action == GLFW_PRESS) {
		if (mods == GLFW_MOD_SHIFT)
			numPhotons -= 10;
		else
			numPhotons--;
		numPhotons = (numPhotons < 0) ? 0 : numPhotons;
		printOGL();
	}
	if (key == GLFW_KEY_RIGHT_BRACKET && action == GLFW_PRESS) {
		if (mods == GLFW_MOD_SHIFT)
			numPhotons += 10;
		else
			numPhotons++;
		printOGL();
	}
}

vector<float> boxColors = {
	1.f, 0.f, 0.f,
	1.f, 0.f, 0.f,
	1.f, 0.f, 0.f,
	1.f, 0.f, 0.f,
	1.f, 0.f, 0.f,
	1.f, 0.f, 0.f,
	1.f, 0.f, 0.f,
	1.f, 0.f, 0.f,
	1.f, 0.f, 0.f,
	1.f, 0.f, 0.f,
	1.f, 0.f, 0.f,
	1.f, 0.f, 0.f,
	1.f, 0.f, 0.f,
	1.f, 0.f, 0.f,
	1.f, 0.f, 0.f,
	1.f, 0.f, 0.f
};


int main(int argc, char** argv) {
	current = 0;
	numPhotons = 0;
	point = Vector3d::Zero();
	string inFile, outFile;
	bool useOGL = false;
	if (argc == 3) {
		if (argv[1][0] == '/') {
			inFile = argv[2];
			useOGL = true;
		}
		else {
			inFile = argv[1];
			outFile = argv[2];
		}
	} else if (argc == 2) {
		inFile = argv[1];
		outFile = inFile.substr(0, inFile.find_last_of('.')) + ".exr";
	} else {



		return 0;
	}
	int	width, height;
	tracer.parse(inFile.c_str());
	width = tracer.width;
	height = tracer.height;
	ratio = width / (float) height;
	if (useOGL) {
		level = 0;
		glfwSetErrorCallback(error_cb);
		if (!glfwInit()) {
			return 1;
		}
		string windowName = "RayTra Viz - " + inFile;
		glfwWindowHint(GLFW_SAMPLES, 4);
		glWindow = glfwCreateWindow(width, height, windowName.c_str(), NULL, NULL);
		if (!glWindow) {
			glfwTerminate();
			return 1;
		}
		glfwMakeContextCurrent(glWindow);
		glfwSetWindowSizeCallback(glWindow, bufferSize_cb);
		glfwSetKeyCallback(glWindow, key_cb);

		glewExperimental = GL_TRUE;
		glewInit();

		const GLubyte* renderer = glGetString(GL_RENDERER);
		const GLubyte* version = glGetString(GL_VERSION);
		cout << "renderer: " << renderer << endl;
		cout << "OpenGL version: " << version << endl;

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		tracer.renderBoundingBoxes(vertices, level);
		vbo.resize(vertices.size());
		GLuint boxColorVbo = 0;
		glGenBuffers(vertices.size(), &vbo[0]);
		for (size_t i = 0; i < vertices.size(); i++) {
			glBindBuffer(GL_ARRAY_BUFFER, vbo[i]);
			glBufferData(GL_ARRAY_BUFFER, vertices[i].size() * sizeof(float), &vertices[i][0], GL_STATIC_DRAW);
		}
		glGenBuffers(1, &boxColorVbo);
		glBindBuffer(GL_ARRAY_BUFFER, boxColorVbo);
		glBufferData(GL_ARRAY_BUFFER, boxColors.size() * sizeof(float), &boxColors[0], GL_STATIC_DRAW);

		GLuint vao = 0;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		tracer.populatePhotonMapOGL();
		tracer.renderPhotonMapOGL(photons, photonColors, 3);

		glGenBuffers(1, &photonVbo);
		glGenBuffers(1, &colorVbo);
		glBindBuffer(GL_ARRAY_BUFFER, photonVbo);
		glBufferData(GL_ARRAY_BUFFER, photons.size() * sizeof(float), &photons[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
		glBufferData(GL_ARRAY_BUFFER, photonColors.size() * sizeof(float), &photonColors[0], GL_STATIC_DRAW);


		GLuint photonVao;
		glGenVertexArrays(1, &photonVao);

		GLuint shaderProgram = createProgram("../../RayTra/vert.glsl", "../../RayTra/frag.glsl");
		GLuint vp_loc = glGetAttribLocation(shaderProgram, "vp");
		GLuint color_loc = glGetAttribLocation(shaderProgram, "input_color");
		GLuint camera_loc = glGetUniformLocation(shaderProgram, "camera");
		
		glClearColor(0.f, 0.f, 0.f, 1.0);

		bufferSize_cb(glWindow, width, height);

		while (!glfwWindowShouldClose(glWindow)) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glEnableVertexAttribArray(vp_loc);
			glEnableVertexAttribArray(color_loc);
			glUseProgram(shaderProgram);
			glUniformMatrix4fv(camera_loc, 1, GL_FALSE, camera.data());

			for (size_t i = 0; i < vertices.size(); i++) {
				glBindVertexArray(vao);
				glBindBuffer(GL_ARRAY_BUFFER, vbo[i]);
				glVertexAttribPointer(vp_loc, 3, GL_FLOAT, GL_FALSE, 0, NULL);
				glBindBuffer(GL_ARRAY_BUFFER, boxColorVbo);
				glVertexAttribPointer(color_loc, 3, GL_FLOAT, GL_FALSE, 0, NULL);
				glDrawArrays(GL_LINE_LOOP, 0, vertices[i].size()/3);
			}

			glBindVertexArray(photonVao);
			glBindBuffer(GL_ARRAY_BUFFER, photonVbo);
			glVertexAttribPointer(vp_loc, 3, GL_FLOAT, GL_FALSE, 0, NULL);
			glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
			glVertexAttribPointer(color_loc, 3, GL_FLOAT, GL_FALSE, 0, NULL);
			glDrawArrays(GL_POINTS, 0, photons.size() / 3);

			glfwSwapBuffers(glWindow);
			glfwWaitEvents();
		}

		glfwDestroyWindow(glWindow);
		glfwTerminate();
		return 0;
	}
	pixels = new unsigned int[width * height];
	for (int i = 0; i < width * height; i++) {
		pixels[i] = 0;
	}
	o.resizeErase (height, width);
	cout << endl << width << " " << height << endl;
	clock_t start = clock();
	tracer.render(o);
	clock_t ends = clock();
	double runtime = (double) (ends - start) / CLOCKS_PER_SEC;
	cout << runtime << " seconds" << endl;
	writeRgba(outFile.c_str(), &o[0][0], width, height);
	return 0;
}
