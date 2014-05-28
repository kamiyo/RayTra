/*
 * main.cpp
 *
 *  Created on: Dec 2, 2011
 *      Author: kamiyo
 */
/*
#ifdef APPLE || _APPLE || APPLE || _APPLE_
#include <GLUT/glut.h>

#else
#include <GL/glut.h>

#endif
*/

#include <fstream>
#include <iostream>
#include <cstring>
#include <memory>
#include "RayTra.h"

using namespace std;


//OpenGL stuff
/*
int glWindow;
bool rendered = false;
*/


// stuff for exr output
unsigned int* pixels;
int width, height;
RayTra tracer;
Imf::Array2D<Imf::Rgba> o;
string name;

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

int main(int argc, char** argv) {
	//clock_t start = clock();
	//glutInit(&argc, argv);
	//glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	if (argc == 3) {
		name = argv[2];
	} else if (argc == 2) {
		name = argv[1];
		name = name.substr(0, name.find_last_of('.')) + ".exr";
	}
	else if (argc == 1) {
		clock_t start = clock();
		Vector3d temp = Vector3d::Zero();
		for (int i = 0; i < 1; i++) {
			Vector3d t(0, 1, 0);
			temp += cosVec(t);
		}
		clock_t ends = clock();
		double runtime = (double) (ends - start) / CLOCKS_PER_SEC;
		cout << temp.normalized() << " " << runtime << endl;

		start = clock();
		temp.setZero();
		for (int i = 0; i < 1; i++) {
			Vector3d t(0, 1, 0);
			temp += _cosVec(t);
		}
		ends = clock();
		runtime = (double) (ends - start) / CLOCKS_PER_SEC;
		cout << temp.normalized() << " " << runtime << endl;
		return 0;
	}
	else {
		cerr << "not correct number of arguments" << endl;
		return 1;
	}
	tracer.parse(argv[1]);
	width = tracer.width;
	height = tracer.height;
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
	writeRgba(name.c_str(), &o[0][0], width, height);
	return 0;
	//glutInitWindowPosition(0,0);
	//glutInitWindowSize(width,height);
	//glWindow = glutCreateWindow("RayTra");
	//glViewport(0, 0, width, height);
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//gluOrtho2D(0, width, 0, height);
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	//glClearColor(0.0, 0.0, 0.0, 0.0);
	//glutDisplayFunc(display);
	//glutIdleFunc(idle);
	//glutKeyboardFunc(keylistener);
	//glDisable(GL_DEPTH_TEST);
	//glutMainLoop();
	//clock_t ends = clock();
	//double runtime = (double) (ends - start) / CLOCKS_PER_SEC;
	//std::cout << "Elapsed: " << runtime << std::endl;
}
