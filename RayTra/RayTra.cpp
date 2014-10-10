/*
 * RayTra.cpp
 *
 *  Created on: Dec 2, 2011
 *      Author: kamiyo
 */

#include "RayTra.h"

void RayTra::renderPhotonMapOGL(std::vector<float>& vertices, std::vector<float>& colors) {
	seedRand();					// seed rand

	if (usePhotonMap) {
		shading->initPhotonTracing(numPhotons / omp_get_max_threads());
		u_ptr<Photons> allPhotons = make_unique<Photons>();
#pragma omp parallel
		{
			u_ptr<Photons> tracedPhotons = shading->tracePhotons(allSurfaces);
#pragma omp critical
			allPhotons->m_photons.insert(allPhotons->m_photons.end(), tracedPhotons->m_photons.begin(), tracedPhotons->m_photons.end());
		}
		std::cout << allPhotons->size() << std::endl;
		photonMap = std::make_unique<PhotonMap>(allPhotons);
		photonMap->drawPhotons(vertices, colors);
	}

}

void RayTra::render(Imf::Array2D<Imf::Rgba>& o) {
	seedRand();					// seed rand
	
	if (usePhotonMap) {
		shading->initPhotonTracing(numPhotons / omp_get_max_threads());
		u_ptr<Photons> allPhotons = make_unique<Photons>();
#pragma omp parallel
		{
			u_ptr<Photons> tracedPhotons = shading->tracePhotons(allSurfaces);
#pragma omp critical
			allPhotons->m_photons.insert(allPhotons->m_photons.end(), tracedPhotons->m_photons.begin(), tracedPhotons->m_photons.end());
		}
		photonMap = std::make_unique<PhotonMap>(allPhotons);
	}

	Sampler master(width, height,
					Sampler::INTEGRAL,
					Sampler::SQUARE,
					((renderOrder == HILBERT) ? (Sampler::HILBERT) : 0),
					Sampler::CENTER,
					false);
	Sampler multiSampler(numSamples, numSamples,
					Sampler::FRACTIONAL,
					Sampler::SQUARE,
					((renderOrder == HILBERT) ? (Sampler::HILBERT) : 0), 
					((numSamples > 1) ? (sampleType) : (Sampler::CENTER)),
					true);
	Sampler lensSampler, lightSampler;

	if (dofSetting == OFF) lensSampler = Sampler(numSamples, numSamples);
	else lensSampler = Sampler(numSamples, numSamples,
							Sampler::FRACTIONAL,
							((dofSetting == CIRCLE) ? (Sampler::CIRCLE) : (Sampler::SQUARE)),
							Sampler::LINEAR,
							((numSamples > 1) ? (sampleType) : (Sampler::CENTER)),
							true);
	if (shadowSetting <= 1) lightSampler = Sampler(numSamples, numSamples);
	else lightSampler = Sampler(numSamples, numSamples,
							Sampler::FRACTIONAL,
							((shadowSetting == SOFTCIRCLE) ? (Sampler::CIRCLE) : (Sampler::SQUARE)),
							Sampler::LINEAR,
							((numSamples > 1) ? (sampleType) : (Sampler::CENTER)),
							true);
	Sampler2d masterPixels = master.genPoints();

	int counter = 0;
	clock_t master_start = clock();
#pragma omp parallel for schedule(dynamic)
	for (int j = 0; j < height; j++) {

		clock_t start = clock();
		int startray = Ray::count;

		for (int i = 0; i < width; i++) {

			Vector3d c = Vector3d::Zero();		// initialize color result vector (RGB)

			Sampler2d lightSample = lightSampler.genPoints();
			Sampler2d lensSample = lensSampler.genPoints();
			Sampler2d multiSample = multiSampler.genPoints();
			Sampler::shuffle(lensSample, true);
			Sampler::shuffle(lightSample, true);

			int x = (int)masterPixels(i, j)[0];
			int y = (int)masterPixels(i, j)[1];
			// multisampling raytracing
			for (int q = 0; q < numSamples; q++) {
				for (int p = 0; p < numSamples; p++) {

					int currentSample = q * numSamples + p;
					Ray r;
					camera->generateRay(lensSample(currentSample), x + multiSample(currentSample)[0], y + multiSample(currentSample)[1], r);
					c += shading->computeShading(r, 0, INF, allSurfaces, lightSample(currentSample));				// SHADE
				}
			}
			c /= (numSamples * numSamples);		// normalize result
			int invHeight = height - (y + 1);
			o[invHeight][x].r = (float) c[0];
			o[invHeight][x].g = (float) c[1];
			o[invHeight][x].b = (float) c[2];
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
			std::cout << "\rprogress: " << floor(perc) << "% : " << counter << " " << Ray::count << " rays - " << ((double) raysince / runtime) << " r/s";
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

RayTra::RayTra() {
	surfaces = make_unique<Group>();
	allSurfaces = make_unique<Group>();
	shading = make_unique<Shading>();
	esf = 0;
	numSamples = 1;
	shadowSetting = OFF;
	accelerationStructure = LIST;
	renderOrder = LINEAR;
	hasNorm = false;
	startMaterial = true;
	dofSetting = OFF;
	circleLight = false;
	actualLights = false;
	usePhotonMap = false;
	numPhotons = 0;
}

RayTra::~RayTra() {}

void RayTra::renderBoundingBoxes(std::vector<std::vector<float> >& verts, int level) const {
	allSurfaces->renderBoundingBox(verts, level);
}

void RayTra::populateLights() {
	size_t size = shading->_l.size();
	for (size_t i = 0; i < size; i++) {
		s_ptr<Light> l = shading->_l[i];
		if (l->_type == Light::POINT) {
			std::cout << "populating lights" << std::endl;
			s_ptr<LightP> p = static_pointer_cast<LightP>(l);
			u_ptr<Surface> s = make_unique<Sphere>(p);
			surfaces->addSurface(s);
		}
	}
}

void RayTra::sphere(Vector3d pos, double r) {
	u_ptr<Surface> s = make_unique<Sphere>(pos, r, prevMaterial);
	applyTransform(s);
	surfaces->addSurface(s);
}
void RayTra::triangle(Vector3d p1, Vector3d p2, Vector3d p3) {
	u_ptr<Surface> t = make_unique<Triangle>(p1, p2, p3, prevMaterial);
	applyTransform(t);
	surfaces->addSurface(t);
}
void RayTra::plane(Vector3d n, Vector3d p){
	u_ptr<Surface> pp = make_unique<Plane>(n, p, prevMaterial);
	allSurfaces->addSurface(pp);
}
void RayTra::circle(Vector3d p, Vector3d n, double r) {
	u_ptr<Surface> c = make_unique<Circle>(p, n, r, prevMaterial);
	applyTransform(c);
	surfaces->addSurface(c);
}
void RayTra::cylinder(double r, double h, char cap) {
	u_ptr<Surface> c = make_unique<Cylinder>(r, h, prevMaterial);
	applyTransform(c);
	if (cap == 'p' || cap == 'b') {
		u_ptr<Surface> top = make_unique<Circle>(Vector3d(0, 0, h / 2.0), Vector3d(0, 0, 1), r, prevMaterial);
		applyTransform(top);
		surfaces->addSurface(top);
	}
	if (cap == 'n' || cap == 'b') {
		u_ptr<Surface> bottom = make_unique<Circle>(Vector3d(0, 0, -h / 2.0), Vector3d(0, 0, -1), r, prevMaterial);
		applyTransform(bottom);
		surfaces->addSurface(bottom);
	}
	surfaces->addSurface(c);
}
void RayTra::cone(double l, double u, char cap) {
	u_ptr<Surface> c = make_unique<Cone>(l, u, prevMaterial);
	applyTransform(c);
	if (cap == 'p' || cap == 'b') {
		if (u != 0) {
			u_ptr<Surface> top = make_unique<Circle>(Vector3d(0, 0, u), Vector3d(0, 0, 1), u, prevMaterial);
			applyTransform(top);
			surfaces->addSurface(top);
		}
	}
	if (cap == 'n' || cap == 'b') {
		if (l != 0) {
			u_ptr<Surface> bottom = make_unique<Circle>(Vector3d(0, 0, l), Vector3d(0, 0, -1), l, prevMaterial);
			applyTransform(bottom);
			surfaces->addSurface(bottom);
		}
	}
	surfaces->addSurface(c);
}
void RayTra::torus(double R, double r) {
	u_ptr<Surface> t = make_unique<Torus>(R, r, prevMaterial);
	applyTransform(t);
	surfaces->addSurface(t);
}

void RayTra::createCamera(Vector3d pos, Vector3d at, Vector3d dir, Vector3d up, Vector3d fp, Vector3d fd, double d, double fl, double iw, double ih, int pw, int ph, double size){
	if (size == nINF) {
		dofSetting = OFF;
	}
	width = pw;
	height = ph;
	camera = make_unique<Camera>(pos, at, dir, up, fp, fd, d, fl, iw, ih, pw, ph, size);
}
void RayTra::pointLight(Vector3d pos, Vector3d rgb, Vector3d atten, double r){
	s_ptr<LightP> l = make_shared<LightP>(pos, rgb, atten, r);
	shading->addLight(l);
}
void RayTra::directionalLight(Vector3d dir, Vector3d rgb, Vector3d atten){
	s_ptr<LightD> l = make_shared<LightD>(dir, rgb, atten);
	shading->addLight(l);
}
void RayTra::spotLight(Vector3d pos, Vector3d dir, double theta, double phi, double p, Vector3d rgb, Vector3d atten, double r){
	s_ptr<LightS> l = make_shared<LightS>(pos, dir, theta, phi, p, rgb, atten, r);
	shading->addLight(l);
}
void RayTra::ambientLight(Vector3d rgb){
	shading->addAmbient(rgb);
}
void RayTra::material(string s) {
	auto iter = mtlMap.find(s);
	if (iter == mtlMap.end()) {
		cerr << "material string does not exist; ignoring" << endl;
	}
	else {
		prevMaterial = (iter)->second;
	}
}
void RayTra::material(Vector3d amb, Vector3d diff, Vector3d spec, double r, Vector3d refl, double n, Vector3d atten){
	prevMaterial = make_shared<Material>(amb, diff, spec, r, refl, n, atten);
	materials.push_back(*prevMaterial);
}
void RayTra::material(string s, Vector3d amb, Vector3d diff, Vector3d spec, double r, Vector3d refl, double n, Vector3d atten) {
	auto iter = mtlMap.find(s);
	if (iter == mtlMap.end()) {
		prevMaterial = make_shared<Material>(amb, diff, spec, r, refl, n, atten);
		mtlMap.insert(pair<string, s_ptr<Material> >(s, prevMaterial));
	} else {
		prevMaterial = make_shared<Material>(amb, diff, spec, r, refl, n, atten);
		(iter)->second = prevMaterial;
		cerr << "material string exists, overwriting old entry" << endl;
	}
}
void RayTra::applyTransform(u_ptr<Surface>& s) {
	if ((T._current != Matrix4d::Identity())) {
		s->trans(T._current, T._currentInv);
	}
}
void RayTra::setOption(int option, int setting, int setting2) {
	switch (option) {
	case SHADOWS:
		shadowSetting = setting;
		if (shadowSetting >= 1)
			shading->_shadows = true;
		break;
	case DOF:
		dofSetting = setting;
		break;
	case SAMPLES:
		if (setting != 0) {
			numSamples = setting;
			sampleType = setting2;
		}
		break;
	case RUSSIAN:
		(setting == 1) ? shading->_russian = true : shading->_russian = false;
		break;
	case REFRACT:
		shading->_refraction = setting;
		break;
	case RECURSE:
		shading->_recurs = setting;
		break;
	case STRUCT:
		accelerationStructure = setting;
		break;
	case INDIRECT:
		shading->_indirect = setting;
		break;
	case ACTUALLIGHTS:
		(setting == 0) ? actualLights = false :	actualLights = true;
		break;
	case ORDER:
		renderOrder = setting;
		break;
	case PHOTONMAPPING:
		numPhotons = setting;
		(setting > 0) ? (usePhotonMap = true) : (usePhotonMap = false);
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
	if (accelerationStructure == BoVoH) {
		u_ptr<Surface> objects = make_unique<BVH>(surfaces);
		allSurfaces->addSurface(objects);
	} else {
		u_ptr<Surface> s = std::move(surfaces);
		allSurfaces->addSurface(s);
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
			auto iter = mtlMap.find(cmd);
			if (iter != mtlMap.end()) {
				prevMaterial = (iter)->second;
			}
		} else if (cmd=="v") {
			Vector3d v;
			iss >> v;
			s_ptr<Vertex> tempvtx(make_shared<Vertex>(v));
			m_v.push_back(tempvtx);
		} else if (cmd=="vn") {
			Vector3d n;
			iss >> n;
			m_n.push_back(n);
			hasNorm = true;
		} else if (cmd=="f") {
			getline(iss, cmd);
			vector<int> e;
			vector<int> nv;
			splitStringToInt(cmd.substr(1), ' ', e);
			splitStringToNormals(cmd.substr(1), ' ', nv);
			size_t n = 2 + e.size() - 4;
			for (size_t i = 0; i < n; i++) {
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
					mtlMap.insert(pair<string, s_ptr<Material> >(name, make_shared<Material>(amb, diff, spec, p, refl, n, atten)));
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
		mtlMap.insert(pair<string, s_ptr<Material> >(name, make_shared<Material>(amb, diff, spec, p, refl, n, atten)));

}

void RayTra::createFace(int v1, int v2, int v3, int n1, int n2, int n3, int smooth) {
	u_ptr<Face> tempF(make_unique<Face>(prevMaterial));
	vector<int> vs;
	vector<int> ns;
	vs.push_back(v1 - 1); vs.push_back(v2 - 1); vs.push_back(v3 - 1);
	ns.push_back(n1 - 1); ns.push_back(n2 - 1); ns.push_back(n3 - 1);
	for (int i = 0; i < 3; i++) {
		int index = vs[(i + 1) % 3];
		s_ptr<HEdge> tempE(make_shared<HEdge>());
		tempE->setVertex(m_v[index]);
		if (hasNorm) {
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
	m_h[esf]->setNext(m_h[esf+1]);
	m_h[esf+1]->setNext(m_h[esf+2]);
	m_h[esf+2]->setNext(m_h[esf]);
	tempF->setHE(m_h[esf]);
	if (!hasNorm) {
		Vector3d p1 = m_h[esf]->getVertex()->_p;
		Vector3d p2 = m_h[esf + 1]->getVertex()->_p;
		Vector3d p3 = m_h[esf + 2]->getVertex()->_p;
		Vector3d norm = ((p2 - p1).cross(p3 - p1)).normalized();
		for (int i = 0; i < 3; i++) {
			m_h[esf + i]->getVertex()->addNormal(norm);
		}
		tempF->_normal = norm;
		if (smooth == 0) {
			tempF->smooth = false;
		} else {
			tempF->smooth = true;
		}
	}
	tempF->boundingBox();
	u_ptr<Surface> tempS = std::move(tempF);
	surfaces->addSurface(tempS);
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
