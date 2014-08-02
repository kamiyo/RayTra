/*
 * RayTra.cpp
 *
 *  Created on: Dec 2, 2011
 *      Author: kamiyo
 */

#include "RayTra.h"

RayTra::RayTra() {
	_surfaces = new Group();
	_all = new Group();
	_shading = new Shading();
	hasnorm = false;
	startMaterial = true;
	structure = LIST;
	field = OFF_0;
	esf = 0;
	samples = 1;
	area = OFF;
	order = LINEAR;
	circular = false;
	circleLight = false;
	actualLights = false;
}

void RayTra::populateLights() {
	int size = _shading->_l.size();
	for (int i = 0; i < size; i++) {
		Light* l = _shading->_l[i];
		if (l->_type == Light::POINT) {
			std::cout << "populating lights" << std::endl;
			LightP* p = (LightP*)l;
			Sphere* s = new Sphere(p);
			_surfaces->addSurface(s);
		}
	}
}

void RayTra::sphere(Vector3d pos, double r) {
	Sphere* s = new Sphere(pos, r, last);
	applyTransform(s);
	_surfaces->addSurface(s);
}
void RayTra::triangle(Vector3d p1, Vector3d p2, Vector3d p3) {
	Triangle* t = new Triangle(p1, p2, p3, last);
	applyTransform(t);
	_surfaces->addSurface(t);
}
void RayTra::plane(Vector3d n, Vector3d p){
	Plane* pp = new Plane(n, p, last);
	_all->addSurface(pp);
}
void RayTra::circle(Vector3d p, Vector3d n, double r) {
	Circle* c = new Circle(p, n, r, last);
	applyTransform(c);
	_surfaces->addSurface(c);
}
void RayTra::cylinder(double r, double h, char cap) {
	Cylinder* c = new Cylinder(r, h, last);
	applyTransform(c);
	if (cap == 'p' || cap == 'b') {
		Circle* top = new Circle(Vector3d(0, 0, h / 2.0), Vector3d(0, 0, 1), r, last);
		applyTransform(top);
		_surfaces->addSurface(top);
	}
	if (cap == 'n' || cap == 'b') {
		Circle* bottom = new Circle(Vector3d(0, 0, -h / 2.0), Vector3d(0, 0, -1), r, last);
		applyTransform(bottom);
		_surfaces->addSurface(bottom);
	}
	_surfaces->addSurface(c);
}
void RayTra::cone(double l, double u, char cap) {
	Cone* c = new Cone(l, u, last);
	applyTransform(c);
	if (cap == 'p' || cap == 'b') {
		if (u != 0) {
			Circle* top = new Circle(Vector3d(0, 0, u), Vector3d(0, 0, 1), u, last);
			applyTransform(top);
			_surfaces->addSurface(top);
		}
	}
	if (cap == 'n' || cap == 'b') {
		if (l != 0) {
			Circle* bottom = new Circle(Vector3d(0, 0, l), Vector3d(0, 0, -1), l, last);
			applyTransform(bottom);
			_surfaces->addSurface(bottom);
		}
	}
	_surfaces->addSurface(c);
}
void RayTra::torus(double R, double r) {
	Torus* t = new Torus(R, r, last);
	applyTransform(t);
	_surfaces->addSurface(t);
}

void RayTra::camera(Vector3d pos, Vector3d at, Vector3d dir, Vector3d up, Vector3d fp, Vector3d fd, double d, double fl, double iw, double ih, int pw, int ph, double size){
	if (size == nINF) {
		field = false;
	}
	width = pw;
	height = ph;
	_cam = new Camera(pos, at, dir, up, fp, fd, d, fl, iw, ih, pw, ph, size);
}
void RayTra::pointLight(Vector3d pos, Vector3d rgb, Vector3d atten, double r){
	LightP* l = new LightP(pos, rgb, atten, r);
	_shading->addLight(l);
}
void RayTra::directionalLight(Vector3d dir, Vector3d rgb, Vector3d atten){
	LightD* l = new LightD(dir, rgb, atten);
	_shading->addLight(l);
}
void RayTra::spotLight(Vector3d pos, Vector3d dir, double theta, double phi, double p, Vector3d rgb, Vector3d atten, double r){
	LightS* l = new LightS(pos, dir, theta, phi, p, rgb, atten, r);
	_shading->addLight(l);
}
void RayTra::ambientLight(Vector3d rgb){
	_shading->addAmbient(rgb);
}
void RayTra::material(string s) {
	map<string, Material*>::iterator iter = mtlMap.find(s);
	if (iter == mtlMap.end()) {
		cerr << "material string does not exist; ignoring" << endl;
	}
	else {
		last = (iter)->second;
	}
}
void RayTra::material(Vector3d amb, Vector3d diff, Vector3d spec, double r, Vector3d refl, double n, Vector3d atten){
	last = new Material(amb, diff, spec, r, refl, n, atten);
	_m.push_back(*last);
}
void RayTra::material(string s, Vector3d amb, Vector3d diff, Vector3d spec, double r, Vector3d refl, double n, Vector3d atten) {
	map<string, Material*>::iterator iter = mtlMap.find(s);
	if (iter == mtlMap.end()) {
		last = new Material(amb, diff, spec, r, refl, n, atten);
		mtlMap.insert(pair<string, Material*>(s, last));
	} else {
		last = new Material(amb, diff, spec, r, refl, n, atten);
		(iter)->second = last;
		cerr << "material string exists, overwriting old entry" << endl;
	}
}
void RayTra::applyTransform(Surface* s) {
	if ((T._current != Matrix4d::Identity())) {
		s->trans(T._current, T._currentInv);
	}
}
void RayTra::setOption(int option, int setting, int setting2) {
	switch (option) {
	case SHADOWS:
		area = setting;
		if (area >= 1) {
			_shading->_shadows = true;
		}
		break;
	case DOF:
		field = setting;
		break;
	case SAMPLES:
		if (setting != 0) {
			samples = setting;
			sample_type = setting2;
		}
		break;
	case RUSSIAN:
		if (setting == 1) {
			_shading->_russian = true;
		}
		break;
	case REFRACT:
		_shading->_refraction = setting;
		break;
	case RECURSE:
		_shading->_recurs = setting;
		break;
	case STRUCT:
		structure = setting;
		break;
	case INDIRECT:
		_shading->_indirect = setting;
		break;
	case LIGHTS:
		if (setting == 0) {
			actualLights = false;
		}
		else {
			actualLights = true;
		}
		break;
	case ORDER:
		order = setting;
		break;
	case CIRCULAR:
		if (setting != 0) {
			circular = true;
		}
	default:
		break;
	}
}
void RayTra::invertNormal() {

}
void RayTra::parse(const char* name){
	Parser::parse(name);
	if (actualLights)
		populateLights();
	if (structure == BoVoH) {
		BVH* objects = new BVH(_surfaces);
		_all->addSurface(objects);
	} else if (structure == BiSpPa) {

	} else {
		_all->addSurface(_surfaces);
	}
}
void RayTra::getObj(const char *file, int smooth) {
	ifstream in(file);
	char buffer[1025];
	string cmd;
	string vertexList = "", faceList = "", normalList = "";
	string l;
	for (int line=1; in.good(); line++) {
		in.getline(buffer,1024);
		buffer[in.gcount()]=0;

		cmd="";

		istringstream iss(buffer);

		vector<string> e;
		iss >> cmd;
		if (cmd[0]=='/' || cmd.empty()) {
			continue;
		} else if (cmd=="mtllib") {
			iss >> cmd;
			parseMtl(cmd.c_str());
		} else if (cmd=="usemtl") {
			iss >> cmd;
			map<string, Material*>::iterator iter = mtlMap.find(cmd);
			if (iter != mtlMap.end()) {
				last = (iter)->second;
			}
		} else if (cmd=="v") {
			Vector3d v;
			iss >> v;
			Vertex* tempvtx = new Vertex(v);
			m_v.push_back(tempvtx);
		} else if (cmd=="vn") {
			Vector3d n;
			iss >> n;
			m_n.push_back(n);
			hasnorm = true;
		} else if (cmd=="f") {
			getline(iss, cmd);
			vector<int> e;
			vector<int> nv;
			splitStringToInt(cmd.substr(1), ' ', e);
			splitStringToNormals(cmd.substr(1), ' ', nv);
			int n = 2 + e.size() - 4;
			for (int i = 0; i < n; i++) {
				createFace(e[0], e[i+1], e[i+2], nv[0], nv[i+1], nv[i+2], smooth);
			}
		}
	} in.close();
}

void RayTra::parseMtl(const char* s) {
	ifstream in(s);
	char buffer[1025];
	string cmd, name = "";
	Vector3d diff, amb, spec, refl, atten; double n = 0;
	diff.setZero(); amb.setZero(); spec.setZero(); refl.setZero(); atten.setZero();
	double p = 2; int illum = 0;

		for (int line=1; in.good(); line++) {
			in.getline(buffer,1024);
			buffer[in.gcount()]=0;
			cmd="";

			istringstream iss(buffer);

			iss >> cmd;
			if (cmd=="newmtl") {
				if (startMaterial == false) {
					if (illum >= 4) {
						refl.setOnes();
					}
					mtlMap.insert(pair<string, Material*>(name, new Material(amb, diff, spec, p, refl, n, atten)));
					diff.setZero(); amb.setZero(); spec.setZero(); refl.setZero(); p = 2; illum = 0;
				} else {
					startMaterial = false;
				}
				iss >> name;
			} else if (cmd=="illum") {
				iss >> illum;
			} else if (cmd=="Kd") {
				iss >> diff;
				//				} else if (cmd=="Ka") {
				//					iss >> amb;
			} else if (cmd=="Ks") {
				iss >> spec;
			} else if (cmd=="Ns") {
				iss >> p;
			} else if (cmd=="Ni") {
				iss >> n;
			} else if (cmd=="Tf") {
				iss >> atten;
			}
		}
		if (illum >= 4) {
			refl.setOnes();
		}
		mtlMap.insert(pair<string, Material*>(name, new Material(amb, diff, spec, p, refl, n, atten)));

}

void RayTra::createFace(int v1, int v2, int v3, int n1, int n2, int n3, int smooth) {
	Face* tempF = new Face(last);
	vector<int> vs;
	vector<int> ns;
	vs.push_back(v1 - 1); vs.push_back(v2 - 1); vs.push_back(v3 - 1);
	ns.push_back(n1 - 1); ns.push_back(n2 - 1); ns.push_back(n3 - 1);
	for (int i = 0; i < 3; i++) {
		int index = vs[(i + 1) % 3];
		HEdge* tempE = new HEdge();
		tempE->setVertex(m_v[index]);
		if (hasnorm) {
			int nindex = ns[(i + 1) % 3];
			m_v[index]->addNormal(m_n[nindex]);
			tempF->_normal = m_n[nindex];
			if (smooth == 0) {
				tempF->smooth = false;
			} else {
				tempF->smooth = true;
			}
		}
		m_h.push_back(tempE);
	}
	m_h[esf]->setFace(tempF);
	m_h[esf]->setNext(m_h[esf+1]);
	m_h[esf+1]->setFace(tempF);
	m_h[esf+1]->setNext(m_h[esf+2]);
	m_h[esf+2]->setFace(tempF);
	m_h[esf+2]->setNext(m_h[esf]);
	tempF->setHE(m_h[esf]);
	if (!hasnorm) {
		Vector3d p1 = m_h[esf]->getVertex()->_p;
		Vector3d p2 = m_h[esf+1]->getVertex()->_p;
		Vector3d p3 = m_h[esf+2]->getVertex()->_p;
		Vector3d norm = ((p2 - p1).cross(p3 - p1)).normalized();
		for (int i = 0; i < 3; i++) {
			m_h[esf+i]->getVertex()->addNormal(norm);
		}
		tempF->_normal = norm;
		if (smooth == 0) {
			tempF->smooth = false;
		} else {
			tempF->smooth = true;
		}
	}
	tempF->boundingBox();
	_surfaces->addSurface(tempF);
	esf += 3;
}

//TODO MAKE THESE BETTER USING >> and << ONLY, NO GETLINE SH&T
void RayTra::splitStringToNormals(const string& s, char d, vector<int>& e) {
	if (s.find_last_of('/') == s.length())
		return;
	stringstream ss(s), i;
	int j;
	string sub;
	while (getline(ss, sub, d)) {
		size_t it = sub.find_last_of('/');
		sub = sub.substr(it + 1);
		i << sub;
		i >> j;
		e.push_back(j);
	}
}

void RayTra::splitStringToInt(const string& s, char d, vector<int>& e) {
	stringstream ss(s), i;
	int j;
	string sub;
	while (getline(ss, sub, d)) {
		i.clear();
		i.str("");
		j = 0;
		i << sub << endl;
		i >> j;
		e.push_back(j);
		sub = "";
	}
}
//TODO make a sampler class and move the sample grid stuff off
// render function
void RayTra::render(Imf::Array2D<Imf::Rgba>& o) {
	init_genrand(time(NULL));
	seedRand();					// seed rand
	Sampler2i bert;
	std::cout << samples << " " << order << std::endl;
	//Vector2i* ples;
	if (order == HILBERT) {
		std::cout << "Hilbert on" << std::endl;
		Hilbert global(width, height);
		bert = global.getPoints();
		//Hilbert sam(samples, samples);
		//ples = sam.getPoints();
	}

	Sampler master(width, height, Sampler::INTEGRAL, Sampler::SQUARE, ((order == HILBERT) ? (Sampler::HILBERT) : 0), Sampler::CENTER, false);
	Sampler ms_sampler(samples, samples, Sampler::FRACTIONAL, Sampler::SQUARE, Sampler::LINEAR, ((samples > 1) ? (sample_type) : (Sampler::CENTER)), true);
	Sampler lens_sampler, light_sampler;
	
	std::cout << area << std::endl;
	if (field == OFF) lens_sampler = Sampler(samples, samples);
	else lens_sampler = Sampler(samples, samples, Sampler::FRACTIONAL, ((circular) ? (Sampler::CIRCLE) : (Sampler::SQUARE)), Sampler::LINEAR, ((samples > 1) ? (sample_type):(Sampler::CENTER)), true);
	if (area <= 1) light_sampler = Sampler(samples, samples);
	else light_sampler = Sampler(samples, samples, Sampler::FRACTIONAL, ((area == SOFTCIRCLE) ? (Sampler::CIRCLE) : (Sampler::SQUARE)), Sampler::LINEAR, ((samples > 1) ? (sample_type) : (Sampler::CENTER)), true);
	Sampler2d master_pixels = master.genPoints();

	int counter = 0;
	clock_t master_start = clock();
#pragma omp parallel for schedule(dynamic)
	for (int j = 0; j < height; j++) {
		
		clock_t start = clock();
		int startray = Ray::count;

		for (int i = 0; i < width; i++) {

			Vector3d c = Vector3d::Zero();		// initialize color result vector (RGB)

			Sampler2d l_sample = light_sampler.genPoints();
			Sampler2d s_sample = lens_sampler.genPoints();
			Sampler2d ms_sample = ms_sampler.genPoints();
			Sampler::shuffle(s_sample, true);
			Sampler::shuffle(l_sample, true);

			int current_pixel = j * width + i;
			int x = master_pixels[current_pixel][0];
			int y = master_pixels[current_pixel][1];
			// multisampling raytracing
			for (int q = 0; q < samples; q++) {
				for (int p = 0; p < samples; p++) {

					int current_sample = q * samples + p;
					int current_pixel = j * width + i;
					Ray r;
					_cam->generateRay(s_sample[current_sample], x + ms_sample[current_sample][0], y + ms_sample[current_sample][1], r);
					c += _shading->computeShading(r, 0, INF, _all, l_sample[current_sample]);				// SHADE
				}
			}
			c = c / (samples * samples);		// normalize result
			int invHeight = height - (y + 1);
			o[invHeight][x].r = c[0];
			o[invHeight][x].g = c[1];
			o[invHeight][x].b = c[2];
			o[invHeight][x].a = 1.0;

			//pixels[j*width + i] = toInt(c[0]) | (toInt(c[1]) << 8) | (toInt(c[2]) << 16);
			//glClear(GL_COLOR | GL_DEPTH);
			//glRasterPos2i(0, 0);
			//glDrawPixels(width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
			//glutSwapBuffers();
#pragma omp atomic
			counter += 1;
		}
		clock_t end = clock();
		double runtime = (double) (end - start) / CLOCKS_PER_SEC;
		double total = (double) (height * width);
		double perc = (double) counter / total;
		int raysince = Ray::count - startray;
		perc *= 100;
#pragma omp critical
		{
			std::cout << "\rprogress: " << floor(perc) << "% : " << counter << " " << Ray::count << " rays - " << ((double)raysince / runtime) << " r/s";
			std::cout.flush();
		}
	}
	clock_t master_stop = clock();
	double master_run = (double) (master_stop - master_start) / CLOCKS_PER_SEC;
	{
		std::cout << "\r\rprogress: 100%, " << Ray::count << " rays at " << ((double) Ray::count / master_run) << " r/s\t\t\t" << endl;
		std::cout.flush();
	}
	return;
}