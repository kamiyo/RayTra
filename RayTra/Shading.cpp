/*
 * Lights.cpp
 *
 *  Created on: Dec 3, 2011
 *      Author: kamiyo
 */

#include "Shading.h"

//constructor sets up default values in case of no parameters
Shading::Shading() {
	init_genrand(time(NULL));
	_recurs = 0;
	_amb = Vector3d::Zero();
	_shadows = false;
	_refraction = 0;
	_indirect = 0;
	_russian = false;
}

Shading::~Shading() {
}

void Shading::setRecursDepth(int d) {
	_recurs = d;
}

// to-do set Refrac depth, set _shadows, etc

void Shading::addLight(Light* l) {
	_l.push_back(l);
}

void Shading::addAmbient(Vector3d a) {
	_amb = a;
}

// form of function where recursion and refraction are not specified. 
Vector3d Shading::computeShading(Ray vray, double t0, double t1, Group* s, const Vector2d& area) {
	return computeShading(vray, t0, t1, s, area, _recurs, _refraction);
}

//SHADER
Vector3d Shading::computeShading(Ray vray, double t0, double t1, Group* s, const Vector2d& area, int recurs, int refrac) {
	if (recurs == -1 || refrac == -1) {
		return Vector3d::Zero();						//return 0 if recursion limit reached
	}
	hitRecord rec, srec;								// rec = light record, srec = shadow record
	Vector3d result;									// rgb result
	result.setZero();
	Vector3d cook;
	cook.setZero();

	Vector3d d = (-1.0 * vray.dir).normalized();									// d = viewing ray direction (out of surface)
	Material* m;

	if (s->_hit(vray, t0, t1, rec)) {
		double epsilon = 0.001 / vray.dir.norm();
		Vector3d n = rec.n;						// n = normal vector of intersection
		
		double nd = n.dot(d);

		if (rec.m == NULL) {
			std::cout << "shouldn't be here unless lightsphere" << std::endl;
			if (rec.l == NULL) return Vector3d::Zero();
			Vector3d light = Vector3d::Zero();
			light += (rec.l->_rgb) * std::max((double) 0, nd);
			return light;
		}

		const Vector3d p = vray.eye + rec.t * vray.dir;				// p = intersection point
		m = rec.m;										// material at intersection

		Vector3d global; global.setZero();
		for (int gi = 0; gi < _indirect; gi++) {
			Vector3d newDir = COSVEC(n);
			Ray diffR(p, vray.dir.norm() * newDir, vray.ref, vray.alpha, Ray::VIEW);
			global += computeShading(diffR, epsilon, INF, s, area, recurs - 1, refrac);
		}

		if (_indirect != 0) {
			//std::cout << _indirect << std::endl;
			result += m->kd.cwiseProduct(global / (2. * (double)_indirect));
		}
		for (int j = 0; j < (int)_l.size(); j++) {		// for each light
			Vector3d l = _l[j]->getVector(p);			//		l = light position
			if (_l[j]->_type == Light::POINT || _l[j]->_type == Light::SPOT){						//		if light is a point
				Vector3d w = l.normalized();			//		find the point on the "area" light
				Vector3d up(0.0, 1.0, 0.0);
				if (w.dot(up) == 1) up << 1.0, 0.0, 0.0;
				Vector3d u = (up.cross(w)).normalized();
				Vector3d v = (w.cross(u)).normalized();
				Vector2d _area = area.array() * ((LightP*)_l[j])->_r;
				l = l + u * _area[0] + v * _area[1];
			}
			double lepsilon = 0.001 / l.norm();
			Ray sRay(p, l, vray.ref, vray.alpha, Ray::SHAD);
			/* if shadows are off or if a shadow is not found
			 calculate lighting
			 I = light's intensity in RGB
			 l = ray to light
			 h = bisector of l and d
			 result += diff*I*max(0, n.l) + spec*I*max(0, n.h)^shininess
			*/
			double fall = _l[j]->getFalloff(p);
			
			if ((fall != 0 && !s->_hit(sRay, lepsilon, 1, srec)) || srec.m == NULL || _shadows == false) {
				Vector3d nn = n;
				if (nd < 0) {
					nd = -nd;
					nn = -nn;
				}
				Vector3d I = _l[j]->_rgb;
				l = l.normalized();
				Vector3d h = (l.normalized() + d).normalized();
				double nl = nn.dot(l);
				double nh = nn.dot(h);
				double dh = nn.dot(h);

				/*double g = std::min((double)1.0, std::min(2 * nh * nd / dh, 2 * nh * nl / dh));
				double alpha = acos(nh);
				double rms = 0.1;
				double beckmann = (1 / (rms * rms * nh * nh * nh * nh)) * exp(-1.0 * (alpha * alpha) / (rms * rms));

				*/
				//
				if (_indirect != 0) {
					result += fall * ((m->kd).cwiseProduct(I) * std::max((double) 0, nl) / 2.0 + (m->ks).cwiseProduct(I) * std::pow(std::max((double) 0, nh), m->p));
				}
				else {
					result += fall * ((m->kd).cwiseProduct(I) * std::max((double) 0, nl) + (m->ks).cwiseProduct(I) * std::pow(std::max((double) 0, nh), m->p));
				}
				//cook += (m->ks).cwiseProduct(I) * (fresnel((double)2.5, dh) * beckmann * g / (M_PI * nd));
				
				//result += aoao * ((m->kd).cwiseProduct(I) * std::max((double)0, nl) + (m->ks).cwiseProduct(I) * pow(std::max((double)0, nh), m->p));
			}
		}
		d = vray.dir;
		// if reflection index is not 0 and if refractions are turned on
		if ((m->n != 0) && _refraction) {
			//std::cout << " in reflections " << std::endl;
			double c1, c2; Vector3d krefract, kreflect; krefract.setZero();
			double current, to;
			Ray v0, v1; v1.ref = vray.ref; v1.alpha = vray.alpha;
			double dnorm = d.norm();
			Vector3d reflect = d - 2 * (d.dot(n)) * n;	// reflect = reflected vector
			d.normalize();								// d = viewing ray direction
			Vector3d t; t.setZero();
			c1 = abs(d.dot(n));
			current = vray.ref.back();
			refrac = refrac - 1;
			krefract << pow(EULER, -1.0 * v1.alpha.back()[0]*rec.t), pow(EULER, -1.0 * v1.alpha.back()[1]*rec.t), pow(EULER, -1.0 * v1.alpha.back()[2]*rec.t);
			n = rec.n;
			if (d.dot(n) < 0) {							// if viewing vector is in front of normal, i.e. entering a refracting object
				//std::cout << "dot positive" << std::endl;
				to = m->n;
				//std::cout << "cur to " << current << " " << to << std::endl;
				v1.ref.push_back(to);
				v1.alpha.push_back(m->a);
				if (refract(d, n, current, to, t)) {
					c2 = abs((t.normalized()).dot(n));
				} else {
					//std::cout << "total internal" << std::endl;
					Ray refRay(p, reflect, vray.ref, vray.alpha, Ray::VIEW);
					//std::cout << v.ref.size() << std::endl;
					//std::cout << "return" << std::endl;
					return result += krefract.cwiseProduct(computeShading(refRay, epsilon, INF, s, area, recurs, refrac));
				}
			} else {
				//std::cout << "dot positive" << std::endl; // exiting a refracting object
				v1.ref.pop_back();
				to = v1.ref.back();
				v1.alpha.pop_back();
				if (refract(d, -1.0 * n, current, to, t)) {
					c2 = abs((t.normalized()).dot(n));
				} else {								// if total internal refraction, then only add reflection
					//std::cout << "total internal" << std::endl;
					Ray refRay(p, reflect, vray.ref, vray.alpha, Ray::VIEW);
					//std::cout << v.ref.size() << std::endl;
					//std::cout << "return" << std::endl;
					return result += krefract.cwiseProduct(computeShading(refRay, epsilon, INF, s, area, recurs, refrac));
				}
			}
			recurs = recurs - 1 ;
			double index = m->n;
			double R = schlicks(to/current, c1);
			//double R = fresnel(current, to, c1, c2);
			//std::cout << R << std::endl;
			//for (int pp = 0; pp < v1.ref.size(); pp++) {
			//	std::cout << v1.ref[pp] << " ";
			//} std::cout << std::endl;
			//std::cout << "ref " << t[0] << " " << t[1] << " " << t[2] << std::endl;
			v0 = Ray(p, reflect, vray.ref, vray.alpha, Ray::VIEW);
			v1 = Ray(p, t, v1.ref, v1.alpha, Ray::VIEW);
			//std::cout << "reflect" << std::endl;
			if (_russian) {
				double roll = RAN;
				if (roll < R) {
					result += krefract.cwiseProduct(R * computeShading(v0, epsilon, INF, s, area, recurs, refrac));
				}
				else {
					result += krefract.cwiseProduct((1 - R) * computeShading(v1, epsilon, INF, s, area, recurs, refrac));
				}
			}
			else {
				result += krefract.cwiseProduct(R * computeShading(v0, epsilon, INF, s, area, recurs, refrac));
				//std::cout << "refract" << std::endl;
				result += krefract.cwiseProduct((1 - R) * computeShading(v1, epsilon, INF, s, area, recurs, refrac));
			}
		} else if (m->ki != Vector3d::Zero()) {
			d = vray.dir;
			Ray refRay(p, d - 2 * (d.dot(n)) * n, vray.ref, vray.alpha, Ray::VIEW);
			result += m->ki.cwiseProduct(computeShading(refRay, epsilon, INF, s, area, recurs - 1, refrac));
		}
		//result += cook / M_PI;
		//std::cout << "return" << std::endl;
		return result;
	}
	//std::cout << "return" << std::endl;
	return result;
}

double Shading::schlicks(double index, double c) {
	double R0 = pow((index - 1) / (index + 1), 2);
	double R = R0 + (1 - R0) * pow ((1 - c), 5);
	return R;
}

double Shading::fresnel(double index1, double index2, double c1, double c2 ) {
	double rp = (index2 * c2 - index1 * c1) / (index2 * c2 + index1 * c1);
	double rs = (index1 * c1 - index2 * c2) / (index1 * c1 + index2 * c2);
	return ((rp * rp) + (rs * rs)) / 2;
}

bool Shading::refract(Vector3d d, Vector3d n, double index, double indext, Vector3d& t) {
	double under = 1.0 - pow(index / indext, 2) * (1 - pow(d.dot(n), 2));
	if (under < 0) {
		return false;
	}
	t = index / indext * (d - n * (d.dot(n))) - (n * sqrt(under));
	return true;
}