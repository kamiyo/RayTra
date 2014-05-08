#include "parse.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

using namespace std;

#define ON 1
#define OFF 0

void Parser::parse(const char *file) {
	ifstream in(file);
	if (!in.is_open()) {
		exit(0);
	}
	char buffer[1025];
	string cmd;
	bool isCam = false;
	bool isOpt = false;
	bool isMat = false;
	bool isLight = false;
	bool skipping = false;
	string matString = "";

	/* cam variables, initialized */
	Vector3d ce,ca,cu; 
	ce.setZero(); ca.setZero(); cu.setZero();
	double cf,iw,ih,cN = nINF; int pw,ph;

	/* mat variables, initialized */
	Vector3d ka,kd,ks,ki,alpha;
	ka.setOnes(); kd.setZero(); ks.setZero(); ki.setZero(); alpha.setZero();
	double r=0,n=1.0;

	/*light variables, uninitialized */
	Vector3d lpos, ldir, lrgb, latten;
	double ltheta, lphi, lp, lrad;

	for (int line = 1; in.good(); line++) {
		in.getline(buffer, 1024);
		buffer[in.gcount()] = 0;

		cmd = "";

		istringstream iss(buffer);

		iss >> cmd;
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
			if (cmd == "e" || cmd == "eye" || cmd == "pos") {
				iss >> ce;
			}
			else if (cmd == "a" || cmd == "at" || cmd == "dir") {
				iss >> ca;
			}
			else if (cmd == "u" || cmd == "up") {
				iss >> cu;
			}
			else if (cmd == "focal" || cmd == "d" || cmd == "distance") {
				iss >> cf;
			}
			else if (cmd == "i" || cmd == "image") {
				iss >> iw >> ih;
			}
			else if (cmd == "p" || cmd == "pixels" || cmd == "dim") {
				iss >> pw >> ph;
			}
			else if (cmd == "f" || cmd == "N" || cmd == "fstop" || cmd == "f-stop") {
				iss >> cN;
			}
			else if (cmd == "c" || cmd == "camera") {
				isCam = false;
				camera(ce, ca, cu, cf, iw, ih, pw, ph, cN);
			}
			else {
				cout << "Parser error: invalid camera option at line " << line << endl;
			}
		}
		else if (isOpt) {
			// TODO set options -> one function setOption(DEFINE, value)
			if (cmd == "s" || cmd == "shadows") {
				int setting;
				iss >> setting;
				setOption(SHADOWS, setting);
			}
			else if (cmd == "ms" || cmd == "samples") {
				int setting;
				iss >> setting;
				setOption(SAMPLES, setting);
			}
			else if (cmd == "refract" || cmd == "rf") {
				int setting;
				iss >> setting;
				setOption(REFRACT, setting);
			}
			else if (cmd == "accel" || cmd == "structure") {
				int setting;
				iss >> setting;
				setOption(STRUCT, setting);
			}
			else if (cmd == "DOF" || cmd == "depth of field" || cmd == "dof") {
				int setting;
				iss >> setting;
				setOption(DOF, setting);
			}
			else if (cmd == "recurse" || cmd == "rc") {
				int setting;
				iss >> setting;
				setOption(RECURSE, setting);
			}
			else if (cmd == "indirect" || cmd == "GI" || cmd == "gi") {
				int setting;
				iss >> setting;
				setOption(INDIRECT, setting);
			}
			else if (cmd == "actualLights" || cmd == "aL") {
				int setting;
				iss >> setting;
				setOption(LIGHTS, setting);
			}
			else if (cmd == "hilbert" || cmd == "h") {
				int setting;
				iss >> setting;
				setOption(ORDER, setting);
			}
			else if (cmd == "circular" || cmd == "c") {
				int setting;
				iss >> setting;
				setOption(CIRCULAR, setting);
			}
			else if (cmd == "o" || cmd == "options") {
				isOpt = false;
			}
		}
		else if (isMat) {
			if (cmd == "kd" || cmd == "diffuse") {
				iss >> kd;
			}
			else if (cmd == "ks" || cmd == "specular") {
				iss >> ks;
			}
			else if (cmd == "ki" || cmd == "ideal") {
				iss >> ki;
			}
			else if (cmd == "ka" || cmd == "ambient") {
				iss >> ka;
			}
			else if (cmd == "p" || cmd == "r" || cmd == "shininess") {
				iss >> r;
			}
			else if (cmd == "n" || cmd == "index" || cmd == "ior" || cmd == "IoR") {
				iss >> n;
			}
			else if (cmd == "a" || cmd == "alpha" || cmd == "attenuation") {
				iss >> alpha;
			}
			else if (cmd == "m" || "material") {
				isMat = false;
				if (matString == "") {
					material(ka, kd, ks, r, ki, n, alpha);
				}
				else {
					material(matString, ka, kd, ks, r, ki, n, alpha);
				}
			}
		}
		else if (isLight) {
			if (cmd == "p" || cmd == "pos") {
				iss >> lpos;
			}
			else if (cmd == "d" || cmd == "a" || cmd == "at" || cmd == "dir") {
				iss >> ldir;
			}
			else if (cmd == "c" || cmd == "color" || cmd == "rgb") {
				iss >> lrgb;
			}
			else if (cmd == "theta" || cmd == "inner" || cmd == "umbra") {
				iss >> ltheta;
				ltheta = ltheta * M_PI / 180;
			}
			else if (cmd == "phi" || cmd == "outer" || cmd == "penumbra") {
				iss >> lphi;
				lphi = lphi * M_PI / 180;
			}
			else if (cmd == "f" || cmd == "falloff") {
				iss >> lp;
			}
			else if (cmd == "atten" || cmd == "coeff" || cmd == "k") {
				iss >> latten;
			}
			else if (cmd == "radius" || cmd == "r") {
				iss >> lrad;
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
			iss >> v >> d;
			T.rotate(v, d);
		}
		else if (cmd == "scale" || cmd == "sc") {
			double x, y, z;
			iss >> x;
			y = z = x;
			iss >> y >> x;
			T.scale(Vector3d(x, y, z));
		}
		else if (cmd == "move" || cmd == "trans" || cmd == "translate") {
			Vector3d v;
			iss >> v;
			T.translate(v);
		}
		else if (cmd == "s" || cmd == "sphere") {
			Vector3d pos; double r;
			iss >> pos >> r;
			sphere(pos, r);
		}
		else if (cmd == "cone" || cmd == "n") {
			double l = 0, u = 0;
			iss >> l >> u;
			if (l > u) {
				std::swap(l, u);
			}
			cone(l, u, ' ');
		}
		else if (cmd == "np" || cmd == "conep" || cmd == "cone+") {
			double l = 0, u = 0;
			iss >> l >> u;
			if (l > u) {
				std::swap(l, u);
			}
			cone(l, u, 'p');
		}
		else if (cmd == "nn" || cmd == "conen" || cmd == "cone-") {
			double l = 0, u = 0;
			iss >> l >> u;
			if (l > u) {
				std::swap(l, u);
			}
			cone(l, u, 'n');
		}
		else if (cmd == "nb" || cmd == "coneb" || cmd == "cone+-") {
			double l = 0, u = 0;
			iss >> l >> u;
			if (l > u) {
				std::swap(l, u);
			}
			cone(l, u, 'b');
		}
		else if (cmd == "torus" || cmd == "u") {
			double R, r;
			iss >> R >> r;
			torus(R, r);
		}
		else if (cmd == "y" || cmd == "cylinder" || cmd == "cyl") {
			double r, h;
			r = 1.0; h = INF;
			iss >> r >> h;
			cylinder(r, h, ' ');
		}
		else if (cmd == "yp" || cmd == "cylinder+") {
			double r, h;
			r = 1.0; h = INF;
			iss >> r >> h;
			cylinder(r, h, 'p');
		}
		else if (cmd == "yn" || cmd == "cylinder-") {
			double r, h;
			r = 1.0; h = INF;
			iss >> r >> h;
			cylinder(r, h, 'n');
		}
		else if (cmd == "yb" || cmd == "cylinder+-") {
			double r, h;
			r = 1.0; h = INF;
			iss >> r >> h;
			cylinder(r, h, 'b');
		}
		else if (cmd == "t" || cmd == "tri" || cmd == "triangle") {
			Vector3d a, b, c;
			iss >> a >> b >> c;
			triangle(a, b, c);
		}
		else if (cmd == "p" || cmd == "plane") {
			Vector3d n; Vector3d p;
			iss >> n >> p;
			plane(n, p);
		}
		else if (cmd == "c" || cmd == "camera") {
			isCam = true;
		}
		else if (cmd=="m" || cmd == "material") {
			isMat = true;
			iss >> matString;
		}
		else if (cmd == "l" || cmd == "light") {
			isLight = true;
			lpos.setZero(); ldir.setZero(); lrgb.setOnes(); latten.setZero(); latten[0] = 1;
			ltheta = 0; lphi = 0; lp = 0; lrad = 0;
		}
		else if (cmd == "i" || cmd == "cir" || cmd == "circle") {
			Vector3d pos, norm; double r;
			iss >> pos >> norm >> r;
			circle(pos, norm, r);
		}
		else if (cmd=="o" || cmd == "options") {
			isOpt = true;
		} else if (cmd=="obj") {
			iss >> cmd; int smooth; iss >>smooth;
			getObj(cmd.c_str(), smooth);
		} else {
			cout << "Parser error: invalid command at line " << line << endl;
		}
	}

	in.close();
}
