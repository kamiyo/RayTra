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


// global vars
		  unsigned int*	pixels;
					int	width, height;
				 RayTra	tracer;
Imf::Array2D<Imf::Rgba> o;
				 string	name;

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
	std::cout << argc << endl;
	if (argc == 3) {
		name = argv[2];
	} else if (argc == 2) {
		name = argv[1];
		name = name.substr(0, name.find_last_of('.')) + ".exr";
	} else {
		Interval i(0, 1);
		cout << i << endl;
		cout << "should be true: " << Interval::intersects(i, Interval(-1, 0.5)) << endl;
		cout << "============= binary Intersects ============" << endl;
		cout << "should be (0, 0.5): " << i.intersect(Interval(-1, 0.5)) << endl;
		cout << "should be (0.2, 0.8): " << i.intersect(Interval(0.2, 0.8)) << endl;
		cout << "should be (0.5, 1): " << i.intersect(Interval(0.5, 4)) << endl;
		cout << "should be (): " << i.intersect(Interval(9, 10)) << endl;
		cout << "should be (): " << i.intersect(Interval()) << endl;
		cout << "============= binary Unions ================" << endl;
		cout << "should be (0, 4) and (0, 4): " << i.unionize(Interval(1, 4)) << " " << i.unionize(Interval(0.5, 4)) << endl;
		cout << "should be {(0, 1), (5, 6)}: " << i.unionize(Interval(5, 6)) << endl;
		cout << "should be {(-5, -3), (0, 1)}: " << i.unionize(Interval(-5, -3)) << endl;
		cout << "should be (-5, 5): " << i.unionize(Interval(-5, 5)) << endl;
		cout << "should be (0, 1): " << i.unionize(Interval()) << endl;
		cout << "============= binary Differences ===========" << endl;
		Interval k(-5, 5);
		cout << "should be {(-5, -2), (2, 5)}: " << k.difference(Interval(-2, 2)) << endl;
		cout << "should be (0, 5): " << k.difference(Interval(-5, 0)) << endl;
		cout << "should be (-5, 0): " << k.difference(Interval(0, 5)) << endl;
		cout << "should be (): " << k.difference(k) << endl;
		cout << "should be (): " << k.difference(Interval(-10, 10)) << endl;
		cout << "should be (-5, 5): " << k.difference(Interval(10, 20)) << endl;
		cout << "should be (-5, 5): " << k.difference(Interval(-20, -10)) << endl;
		cout << "should be (-5, 5): " << k.difference(Interval()) << endl;
		cout << "should be (): " << Interval().difference(k) << endl;
		cout << "============= Multiple Unions ==============" << endl;
		Intervals j;
		j.push_back(Interval(-1, 1));
		j.push_back(Interval(1.5, 2));
		j.push_back(Interval(2.5, 3));
		//j = Interval::unionize(j);
		cout << "{(-1, 1), (1.5, 2), (2.5, 3)}: " << j << endl;
		Intervals x = j;
		x.push_back(Interval(2, 4));
		//x = Interval::unionize(x);
		cout << "{(-1, 1), (1.5, 4)}: " << x << endl;
		x = j;
		x.push_back(Interval(-5, 5));
		//cout << "{(-5, 5)}: " << Interval::unionize(x) << endl;
		Intervals y; y.push_back(Interval(0, 2)); y.push_back(Interval(3, 4)); y.push_back(Interval(-3, -2));
		x.pop_back(); x.insert(x.end(), y.begin(), y.end());
		//cout << x << endl << Interval::unionize(x) << endl;
		exit(0);
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
