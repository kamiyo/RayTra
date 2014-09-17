#include "parse.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

using namespace std;

#define ON 1
#define OFF 0

void parseError(string s, int l) {
	cerr << "parse error in " << s << " at line " << l << endl;
}

void Parser::parse(const char *file) {
	ifstream _in(file);
	istringstream _iss;
	char buffer[1025];
	if (!_in.is_open()) {
		exit(0);
	}
	string cmd;
	bool isCam = false;
	bool isOpt = false;
	bool isMat = false;
	bool isLight = false;
	bool skipping = false;
	bool isTriStrip = false;
	bool isTri = false;
	bool isQuad = false;
	bool isCyl = false;
	bool isCone = false;
	bool isTor = false;
	bool isCirc = false;
	bool isSphere = false;
	bool isTriFan = false;
	bool isPlane = false;
	bool isCuboid = false;
	int oddeven = 0;
	string matString = "";
	Vector3d nInfVec(nINF, nINF, nINF);
	/* cam variables, initialized */
	Vector3d ce, ca, cu, cd, fp, fd;
	ce = ca = cu = cd = fp = fd = Vector3d(nINF, nINF, nINF);
	double cf = nINF, fl = nINF, iw, ih, cN = nINF; int pw, ph;

	/* mat variables, initialized */
	Vector3d ka, kd, ks, ki, alpha;
	ka.setOnes(); kd.setZero(); ks.setZero(); ki.setZero(); alpha.setZero();
	double r = 0, n = 0;

	/*light variables, uninitialized */
	Vector3d lpos, ldir, lrgb, latten;
	double ltheta, lphi, lp, lrad;

	/*vertex variables*/
	Vector3d va, vb, vc, vd, ve, vf, vg, vh;

	/*position, radius, height*/
	Vector3d pos, norm; double rad, Rad, height, left, right;
	char opt;

	for (int line = 1; _in.good(); line++) {
		_in.getline(buffer, 1024);
		buffer[_in.gcount()] = 0;

		cmd = "";

		_iss = istringstream(buffer);

		_iss >> cmd;
		if (cmd == "/" || cmd.empty()){
			continue;
		}
		else if (skipping) {
			if (cmd == "*/") {
				skipping = false;
			}
		}
		else if (cmd == "/*") {
			skipping = true;
		}
		else if (cmd[0] == '/') {
			continue;
		}
		else if (isCam) {
			if (cmd == "ep" || cmd == "eye" || cmd == "pos") {
				_iss >> ce;
			}
			else if (cmd == "ip" || cmd == "at") {
				_iss >> ca;
			}
			else if (cmd == "id" || cmd == "dir") {
				_iss >> cd;
			}
			else if (cmd == "fp" || cmd == "focal") {
				_iss >> fp;
			}
			else if (cmd == "fd") {
				_iss >> fd;
			}
			else if (cmd == "u" || cmd == "up") {
				_iss >> cu;
			}
			else if (cmd == "d" || cmd == "distance") {
				_iss >> cf;
			}
			else if (cmd == "fl" || cmd == "focal length") {
				_iss >> fl;
			}
			else if (cmd == "i" || cmd == "image") {
				_iss >> iw >> ih;
			}
			else if (cmd == "p" || cmd == "pixels" || cmd == "dim") {
				_iss >> pw >> ph;
			}
			else if (cmd == "f" || cmd == "N" || cmd == "fstop" || cmd == "f-stop") {
				_iss >> cN;
			}
			else if (cmd == "c" || cmd == "camera" || cmd == "}") {
				isCam = false;
				camera(ce, ca, cd, cu, fp, fd, cf, fl, iw, ih, pw, ph, cN);
			}
		}
		else if (isOpt) {
			if (cmd == "s" || cmd == "shadows") {
				int setting;
				_iss >> setting;
				setOption(SHADOWS, setting);
			}
			else if (cmd == "ms" || cmd == "samples") {
				int setting, setting2 = 0;
				_iss >> setting >> setting2;
				setOption(SAMPLES, setting, setting2);
			}
			else if (cmd == "refract" || cmd == "rf") {
				int setting;
				_iss >> setting;
				setOption(REFRACT, setting);
			}
			else if (cmd == "accel" || cmd == "structure") {
				int setting;
				_iss >> setting;
				setOption(STRUCT, setting);
			}
			else if (cmd == "DOF" || cmd == "depth of field" || cmd == "dof") {
				int setting;
				_iss >> setting;
				setOption(DOF, setting);
			}
			else if (cmd == "recurse" || cmd == "rc") {
				int setting;
				_iss >> setting;
				setOption(RECURSE, setting);
			}
			else if (cmd == "indirect" || cmd == "GI" || cmd == "gi") {
				int setting;
				_iss >> setting;
				setOption(INDIRECT, setting);
			}
			else if (cmd == "actualLights" || cmd == "aL") {
				int setting;
				_iss >> setting;
				setOption(LIGHTS, setting);
			}
			else if (cmd == "hilbert" || cmd == "h") {
				int setting;
				_iss >> setting;
				setOption(ORDER, setting);
			}
			else if (cmd == "circular" || cmd == "c") {
				int setting;
				_iss >> setting;
				setOption(CIRCULAR, setting);
			}
			else if (cmd == "russian" || cmd == "rr") {
				int setting;
				_iss >> setting;
				setOption(RUSSIAN, setting);
			}
			else if (cmd == "o" || cmd == "options" || cmd == "}") {
				isOpt = false;
			}
		}
		else if (isMat) {
			if (cmd == "kd" || cmd == "diffuse") {
				_iss >> kd;
			}
			else if (cmd == "ks" || cmd == "specular") {
				_iss >> ks;
			}
			else if (cmd == "ki" || cmd == "ideal") {
				_iss >> ki;
			}
			else if (cmd == "ka" || cmd == "ambient") {
				_iss >> ka;
			}
			else if (cmd == "p" || cmd == "r" || cmd == "shininess") {
				_iss >> r;
			}
			else if (cmd == "n" || cmd == "index" || cmd == "ior" || cmd == "IoR") {
				_iss >> n;
			}
			else if (cmd == "a" || cmd == "alpha" || cmd == "attenuation") {
				_iss >> alpha;
			}
			else if (cmd == "m" || "material" || cmd == "}") {
				isMat = false;
				string tryagain; _iss >> tryagain;
				if (matString == "" || tryagain == "") {
					material(ka, kd, ks, r, ki, n, alpha);
				}
				else {
					if (matString == "") {
						matString = tryagain;
					}
					material(matString, ka, kd, ks, r, ki, n, alpha);
				}
			}
		}
		else if (isLight) {
			if (cmd == "p" || cmd == "pos") {
				_iss >> lpos;
			}
			else if (cmd == "d" || cmd == "a" || cmd == "at" || cmd == "dir") {
				_iss >> ldir;
			}
			else if (cmd == "c" || cmd == "color" || cmd == "rgb") {
				_iss >> lrgb;
			}
			else if (cmd == "theta" || cmd == "inner" || cmd == "umbra") {
				_iss >> ltheta;
				ltheta = ltheta * M_PI / 180;
			}
			else if (cmd == "phi" || cmd == "outer" || cmd == "penumbra") {
				_iss >> lphi;
				lphi = lphi * M_PI / 180;
			}
			else if (cmd == "f" || cmd == "falloff") {
				_iss >> lp;
			}
			else if (cmd == "atten" || cmd == "coeff" || cmd == "k") {
				_iss >> latten;
			}
			else if (cmd == "radius" || cmd == "r") {
				_iss >> lrad;
			}
			else if (cmd == "point") {
				pointLight(lpos, lrgb, latten, lrad);
				isLight = false;
			}
			else if (cmd == "directional") {
				directionalLight(ldir, lrgb, latten);
				isLight = false;
			}
			else if (cmd == "spot") {
				spotLight(lpos, ldir, ltheta, lphi, lp, lrgb, latten, lrad);
				isLight = false;
			}
			else if (cmd == "ambient") {
				ambientLight(lrgb);
				isLight = false;
			}
			else if (cmd == "}") {
				string next = "";
				_iss >> next;
				if (next == "point") {
					pointLight(lpos, lrgb, latten, lrad);
				}
				else if (next == "directional") {
					directionalLight(ldir, lrgb, latten);
				}
				else if (next == "spot") {
					spotLight(lpos, ldir, ltheta, lphi, lp, lrgb, latten, lrad);
				}
				else if (next == "ambient") {
					ambientLight(lrgb);
				}
				else {
					cerr << "need to specify light type after '}'; ignoring" << endl;
				}
				isLight = false;
			}
		}
		else if (isTri) {
			if (cmd == "v" || cmd == "vertex") {
				if ((va.array() == nINF).all()) {
					_iss >> va;
				}
				else if ((vb.array() == nINF).all()) {
					_iss >> vb;
				}
				else if ((vc.array() == nINF).all()) {
					_iss >> vc;
					triangle(va, vb, vc);
					va = vb = vc = nInfVec;
				}
			}
			else if (cmd == "t" || cmd == "tri" || cmd == "triangle" || cmd == "}") {
				isTri = false;
			}
		}
		else if (isQuad) {
			if (cmd == "v" || cmd == "vertex") {
				if ((va.array() == nINF).all()) {
					_iss >> va;
				}
				else if ((vb.array() == nINF).all()) {
					_iss >> vb;
				}
				else if ((vc.array() == nINF).all()) {
					_iss >> vc;
				}
				else if ((vd.array() == nINF).all()) {
					_iss >> vd;
					triangle(va, vb, vc);
					triangle(vc, vb, vd);
					va = vb = vc = vd = nInfVec;
				}
			}
			else if (cmd == "q" || cmd == "quad" || cmd == "quadrilateral" || cmd == "}") {
				isQuad = false;
			}
		}
		else if (isTriStrip) {
			if (cmd == "v" || cmd == "vertex") {
				if ((va.array() == nINF).all()) {
					_iss >> va;
				}
				else if ((vb.array() == nINF).all()) {
					_iss >> vb;
				}
				else if ((vc.array() == nINF).all()) {
					_iss >> vc;
					if (oddeven % 2) {
						triangle(va, vb, vc);
					}
					else {
						triangle(vb, va, vc);
					}
					va = vb;
					vb = vc;
					vc = nInfVec;
					oddeven++;
				}
			}
			else if (cmd == "ts" || cmd == "tristrip" || cmd == "tstrip" || cmd == "}") {
				isQuad = false;
			}
		}
		else if (isTriFan) {
			if (cmd == "v" || cmd == "vertex") {
				if ((va.array() == nINF).all()) {
					_iss >> va;
				}
				else if ((vb.array() == nINF).all()) {
					_iss >> vb;
				}
				else if ((vc.array() == nINF).all()) {
					_iss >> vc;
					triangle(va, vb, vc);
					vb = vc;
					vc = nInfVec;
				}
			}
			else if (cmd == "ts" || cmd == "tristrip" || cmd == "tstrip" || cmd == "}") {
				isQuad = false;
			}
		}
		else if (isSphere) {
			if (cmd == "p" || cmd == "pos") {
				_iss >> pos;
			}
			else if (cmd == "r" || cmd == "rad") {
				_iss >> rad;
			}
			else if (cmd == "s" || cmd == "sphere" || cmd == "}") {
				sphere(pos, rad);
				isSphere = false;
			}
		}
		else if (isCyl) {
			if (cmd == "r" || cmd == "rad") {
				_iss >> rad;
			}
			else if (cmd == "h" || cmd == "height") {
				_iss >> height;
			}
			else if (cmd == "y" || cmd == "cyl" || cmd == "}") {
				cylinder(rad, height, opt);
				isCyl = false;
			}
		}
		else if (isCirc) {
			if (cmd == "p" || cmd == "pos") {
				_iss >> pos;
			}
			else if (cmd == "n" || cmd == "norm") {
				_iss >> norm;
			}
			else if (cmd == "r" || cmd == "rad") {
				_iss >> rad;
			}
			else if (cmd == "i" || cmd == "circle" || cmd == "}") {
				circle(pos, norm, rad);
				isCirc = false;
			}
		}
		else if (isTor) {
			if (cmd == "R") {
				_iss >> Rad;
			}
			else if (cmd == "r") {
				_iss >> rad;
			}
			else if (cmd == "u" || cmd == "torus" || cmd == "}") {
				torus(Rad, rad);
				isTor = false;
			}
		}
		else if (isPlane) {
			if (cmd == "p" || cmd == "pos") {
				_iss >> pos;
			}
			else if (cmd == "n" || cmd == "norm") {
				_iss >> norm;
			}
			else if (cmd == "p" || cmd == "plane" || cmd == "}") {
				plane(norm, pos);
				isPlane = false;
			}
		}
		else if (isCone) {
			if (cmd == "left" || cmd == "l") {
				_iss >> left;
			}
			else if (cmd == "right" || cmd == "r") {
				_iss >> right;
			}
			else if (cmd == "n" || cmd == "cone" || cmd == "}") {
				cone(left, right, opt);
				isCone = false;
			}
		}
		else if (isCuboid) {
			if (cmd == "v" || cmd == "vertex") {
				if (va == nInfVec) {
					_iss >> va;
				}
				else if (vb == nInfVec) {
					_iss >> vb;
				}
				else if (vc == nInfVec) {
					_iss >> vc;
				}
				else if (vd == nInfVec) {
					_iss >> vd;
				}
				else if (ve == nInfVec) {
					_iss >> ve;
				}
				else if (vf == nInfVec) {
					_iss >> vf;
				}
				else if (vg == nInfVec) {
					_iss >> vg;
				}
				else if (vh == nInfVec) {
					_iss >> vh;
				}
			}
			else if (cmd == "cuboid" || cmd == "b" || cmd == "box" || cmd == "}") {
				// top
				triangle(va, vb, vc);
				triangle(va, vc, vd);
				// bottom
				triangle(vf, ve, vh);
				triangle(vf, vh, vg);
				// front
				triangle(vd, vc, vg);
				triangle(vd, vg, vh);
				// back
				triangle(vb, va, ve);
				triangle(vb, ve, vf);
				// right
				triangle(va, vd, vh);
				triangle(va, vh, ve);
				// left
				triangle(vc, vb, vf);
				triangle(vc, vf, vg);
				isCuboid = false;
			}
		}
		else if (cmd == "push") {
			T.push();
		}
		else if (cmd == "pop") {
			T.pop();
		}
		else if (cmd == "identity") {
			T.setIdentity();
		}
		else if (cmd == "rot" || cmd == "rotate") {
			Vector3d v; double d;
			_iss >> v >> d;
			T.rotate(v, d);
		}
		else if (cmd == "scale" || cmd == "sc") {
			double x, y, z;
			_iss >> x;
			y = z = x;
			_iss >> y >> z;
			T.scale(Vector3d(x, y, z));
		}
		else if (cmd == "move" || cmd == "trans" || cmd == "translate") {
			Vector3d v;
			_iss >> v;
			T.translate(v);
		}
		else if (cmd == "s" || cmd == "sphere") {
			Vector3d p = nInfVec; double r;
			_iss >> p >> r;
			if (p == nInfVec) {
				isSphere = true;
				p == nInfVec; r = nINF;
			}
			else {
				sphere(p, r);
			}
		}
		else if (cmd == "cone" || cmd == "n") {
			double l = nINF, u = 0;
			_iss >> l >> u;
			if (l == nINF) {
				isCone = true;
				left = right = nINF;
				opt = ' ';
			}
			else {
				if (l > u) {
					std::swap(l, u);
				}
				cone(l, u, ' ');
			}
		}
		else if (cmd == "np" || cmd == "conep" || cmd == "cone+" || cmd == "n+") {
			double l = nINF, u = 0;
			_iss >> l >> u;
			if (l == nINF) {
				isCone = true;
				left = right = nINF;
				opt = 'p';
			}
			else {
				if (l > u) {
					std::swap(l, u);
				}
				cone(l, u, 'p');
			}
		}
		else if (cmd == "nn" || cmd == "conen" || cmd == "cone-" || cmd == "n-") {
			double l = nINF, u = 0;
			_iss >> l >> u;
			if (l == nINF) {
				isCone = true;
				left = right = nINF;
				opt = 'n';
			}
			else {
				if (l > u) {
					std::swap(l, u);
				}
				cone(l, u, 'n');
			}
		}
		else if (cmd == "nb" || cmd == "coneb" || cmd == "cone+-" || cmd == "n+-") {
			double l = nINF, u = 0;
			_iss >> l >> u;
			if (l == nINF) {
				isCone = true;
				left = right = nINF;
				opt = 'b';
			}
			else {
				if (l > u) {
					std::swap(l, u);
				}
				cone(l, u, 'b');
			}
		}
		else if (cmd == "torus" || cmd == "u") {
			double R, r; R = nINF;
			_iss >> R >> r;
			if (R == nINF) {
				isTor = true;
				Rad = rad = nINF;
			}
			else {
				torus(R, r);
			}
		}
		else if (cmd == "y" || cmd == "cylinder" || cmd == "cyl") {
			double r, h;
			r = nINF; h = INF;
			_iss >> r >> h;
			if (r == nINF) {
				isCyl = true;
				rad = height = nINF;
				opt = ' ';
			}
			else {
				cylinder(r, h, ' ');
			}
		}
		else if (cmd == "yp" || cmd == "cylinder+" || cmd == "y+") {
			double r, h;
			r = nINF; h = INF;
			_iss >> r >> h;
			if (r == nINF) {
				isCyl = true;
				rad = height = nINF;
				opt = 'p';
			}
			else {
				cylinder(r, h, 'p');
			}
		}
		else if (cmd == "yn" || cmd == "cylinder-" || cmd == "y-") {
			double r, h;
			r = nINF; h = INF;
			_iss >> r >> h;
			if (r == nINF) {
				isCyl = true;
				rad = height = nINF;
				opt = 'n';
			}
			else {
				cylinder(r, h, 'n');
			}
		}
		else if (cmd == "yb" || cmd == "cylinder+-" || cmd == "y+-") {
			double r, h;
			r = nINF; h = INF;
			_iss >> r >> h;
			if (r == nINF) {
				isCyl = true;
				rad = height = nINF;
				opt = 'b';
			}
			else {
				cylinder(r, h, 'b');
			}
		}
		else if (cmd == "t" || cmd == "tri" || cmd == "triangle") {
			Vector3d a, b, c; a = nInfVec;
			_iss >> a >> b >> c;
			if (a == nInfVec) {
				isTri = true;
				va = vb = vc = nInfVec;
			}
			else {
				triangle(a, b, c);
			}
		}
		else if (cmd == "q" || cmd == "quad" || cmd == "quadrilateral") {
			Vector3d a, b, c, d; a = nInfVec;
			_iss >> a >> b >> c >> d;
			if (a == nInfVec) {
				isQuad = true;
				va = vb = vc = vd = nInfVec;
			}
			else {
				triangle(a, b, c);
				triangle(a, c, d);
			}
		}
		else if (cmd == "tstrip" || cmd == "tristrip" || cmd == "ts") {
			oddeven = 0;
			isTriStrip = true;
			va = vb = vc = nInfVec;
		}
		else if (cmd == "tfan" || cmd == "tri-fan" || cmd == "tf") {
			isTriFan = true;
			va = vb = vc = nInfVec;
		}
		else if (cmd == "p" || cmd == "plane") {
			Vector3d n = nInfVec, p;
			_iss >> n >> p;
			if (n == nInfVec) {
				isPlane = true;
				pos = norm = nInfVec;
			}
			else {
				plane(n, p);
			}
		}
		else if (cmd == "c" || cmd == "camera") {
			isCam = true;
		}
		else if (cmd == "m" || cmd == "material") {
			isMat = true;
			matString == "";
			ka.setOnes(); kd.setZero(); ki.setZero(); ks.setZero();
			alpha.setZero(); n = 0; r = 0.;
			_iss >> matString;
			if (matString != "" && matString != "{") {
				material(matString);
			}
		}
		else if (cmd == "l" || cmd == "light") {
			isLight = true;
			lpos.setZero(); ldir.setZero(); lrgb.setOnes(); latten.setZero(); latten[0] = 1;
			ltheta = 0; lphi = 0; lp = 0; lrad = 0;
		}
		else if (cmd == "b" || cmd == "box" || cmd == "cuboid") {
			Vector3d a, b, c, d, e, f, g, h; a = b = c = d = e = f = g = h = nInfVec;
			if (a == nInfVec) {
				isCuboid = true;
				va = vb = vc = vd = vf = vg = vh = nInfVec;
			}
			else {
				if (c == nInfVec) {
					Vector3d min = a, max = b;
					d = b;
					f = a;
					a << max[0], max[1], min[2];
					b << min[0], max[1], min[2];
					c << min[0], max[1], max[2];

					e << max[0], min[1], min[2];
					g << min[0], min[1], max[2];
					h << max[0], min[1], max[2];

				}
				// min and max
				// top
				triangle(a, b, c);
				triangle(a, c, d);
				// bottom
				triangle(f, e, h);
				triangle(f, h, g);
				// front
				triangle(d, c, g);
				triangle(d, g, h);
				// back
				triangle(b, a, e);
				triangle(b, e, f);
				// right
				triangle(a, d, h);
				triangle(a, h, e);
				// left
				triangle(c, b, f);
				triangle(c, f, g);
			}
		}
		else if (cmd == "i" || cmd == "cir" || cmd == "circle") {
			Vector3d p, n; double r = nINF;
			_iss >> p >> n >> r;
			if (r == nINF) {
				isCirc = true;
				pos = norm = nInfVec; rad = nINF;
			}
			else {
				circle(p, n, r);
			}
		}
		else if (cmd == "o" || cmd == "options") {
			isOpt = true;
		}
		else if (cmd == "obj") {
			_iss >> cmd; int smooth; _iss >> smooth;
			getObj(cmd.c_str(), smooth);
		}
		else {
			cout << "Parser error: invalid command at line " << line << endl;
		}
	}

	_in.close();
}

