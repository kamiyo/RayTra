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

void Shading::initPhotonTracing() {
	Eigen::MatrixXd ints(3, _l.size());
	for (int i = 0; i < (int) _l.size(); i++) {
		ints.col(i) << _l[i]->_rgb;
	}
	double sum = ints.sum();
	ints /= sum;
	for (int i = 1; i < ints.size(); i++) {
		ints(i) += ints(i - 1);
	}
	_lProbs = ints.array();
}

Photon Shading::emitPhoton() {
	double roll = RAN;
	int color, light;
	for (int i = 0; i < _lProbs.size(); i++) {
		if (roll < _lProbs[i]) {
			color = i % 3;
			light = i / 3;
		}
		else {
			break;
		}
	}
	return _l[light]->emitPhoton(color);
}

std::vector<Photon> Shading::tracePhotons(Group* s) {
	std::vector<Photon> result;
	for (int i = 0; i < _numPhotons; i++) {
		//s->hit();
	}
	return result;
}


// form of function where recursion and refraction are not specified. 
Vector3d Shading::computeShading(Ray vray, double t0, double t1, Group* s, const Vector2d& area) {
	return computeShading(vray, t0, t1, s, area, _recurs, _refraction);
}

//SHADER
Vector3d Shading::computeShading(Ray vray, double t0, double t1, Group* s, const Vector2d& area, int recurs, int refrac) {
	if (recurs == -1 || refrac == -1) {
		return Vector3d::Zero();									//return 0 if recursion limit reached
	}
	hitRecord rec, srec;											// rec = light record, srec = shadow record
	Vector3d result = Vector3d::Zero();								// rgb result zero'd
	Vector3d cook = Vector3d::Zero();

	Vector3d d = (-1.0 * vray.dir).normalized();					// d = viewing ray direction (out of surface)
	Material* m;

	if (s->_hit(vray, t0, t1, rec)) {
		double epsilon = vray.epsilon;
		Vector3d n = rec.n;											// n = normal vector of intersection
		
		double nd = n.dot(d);

		if (rec.m == NULL) {
			std::cout << "shouldn't be here unless lightsphere" << std::endl;
			if (rec.l == NULL) return Vector3d::Zero();
			Vector3d light = Vector3d::Zero();
			light += (rec.l->_rgb) * std::max((double) 0, nd);
			return light;
		}

		const Vector3d p = vray.eye + rec.t * vray.dir;				// p = intersection point
		m = rec.m;													// material at intersection

		Vector3d global; global.setZero();
		for (int gi = 0; gi < _indirect; gi++) {
			Vector3d newDir = COSVEC(n);
			Ray diffR(p, vray.dir.norm() * newDir, vray.ref, vray.alpha, Ray::VIEW);
			global += computeShading(diffR, epsilon, INF, s, area, recurs - 1, refrac);
		}

		if (_indirect != 0) {
			result += m->kd.cwiseProduct(global / (2. * (double)_indirect));
		}
		for (int j = 0; j < (int)_l.size(); j++) {					// for each light
			Vector3d l = _l[j]->getVector(p);						//		l = light position
			double fall = _l[j]->getFalloff(p);
			Vector3d I = _l[j]->_rgb;

			if (_l[j]->_type == Light::POINT || _l[j]->_type == Light::SPOT){		//		if light is a point
				Vector3d w = l.normalized();						//		find the point on the "area" light
				Vector3d up(0.0, 1.0, 0.0);
				if (w.dot(up) == 1) up << 1.0, 0.0, 0.0;
				Vector3d u = (up.cross(w)).normalized();
				Vector3d v = (w.cross(u)).normalized();
				Vector2d _area = area.array() * ((LightP*)_l[j])->_r;
				l = l + u * _area[0] + v * _area[1];
			}

			Ray sRay(p, l, vray.ref, vray.alpha, Ray::SHAD);
			/* if shadows are off or if a shadow is not found
			 calculate lighting
			 I = light's intensity in RGB
			 l = ray to light
			 h = bisector of l and d
			 result += diff*I*max(0, n.l) + spec*I*max(0, n.h)^shininess
			*/
			if ((fall != 0 && !s->_hit(sRay, sRay.epsilon, 1, srec)) || srec.m == NULL || _shadows == false) {
				Vector3d nn = n;
				if (nd < 0) {
					nd = -nd;
					nn = -nn;
				}
				l.normalize();
				Vector3d h = (l + d).normalized();
				double nl = nn.dot(l);
				double nh = nn.dot(h);
				if (_indirect != 0) {
					result += fall * ((m->kd).cwiseProduct(I) * std::max((double) 0, nl) / 2.0 + (m->ks).cwiseProduct(I) * std::pow(std::max((double) 0, nh), m->p));
				}
				else {
					result += fall * ((m->kd).cwiseProduct(I) * std::max((double) 0, nl) + (m->ks).cwiseProduct(I) * std::pow(std::max((double) 0, nh), m->p));
				}
			}
		}
		d = vray.dir;
		// if reflection index is not 0 and if refractions are turned on
		if ((m->n != 0) && _refraction) {
			double c1, c2;
			Vector3d krefract = Vector3d::Zero(), kreflect = Vector3d::Zero();
			double current, to;
			auto temp_ref = vray.ref;
			auto temp_alpha = vray.alpha;
			double dnorm = d.norm();
			Vector3d reflect = d - 2 * (d.dot(n)) * n;	// reflect = reflected vector
			d.normalize();								// d = viewing ray direction
			Vector3d t = Vector3d::Zero();
			c1 = abs(d.dot(n));
			current = vray.ref.back();
			refrac--;
			krefract = (-1.0 * temp_alpha.back()).array().exp();
			n = rec.n;
			if (d.dot(n) < 0) {							// if viewing vector is in front of normal, i.e. entering a refracting object
				to = m->n;
				temp_ref.push_back(to);
				temp_alpha.push_back(m->a);
				if (refract(d, n, current, to, t)) {
					c2 = abs((t.normalized()).dot(n));
				} else {
					Ray refRay(p, reflect, vray.ref, vray.alpha, Ray::VIEW);
					return result += krefract.cwiseProduct(computeShading(refRay, epsilon, INF, s, area, recurs, refrac));
				}
			} else {
				// exiting a refracting object
				temp_ref.pop_back();
				to = temp_ref.back();
				temp_alpha.pop_back();
				if (refract(d, -1.0 * n, current, to, t)) {
					c2 = abs((t.normalized()).dot(n));
				} else {								// if total internal refraction, then only add reflection
					Ray refRay(p, reflect, vray.ref, vray.alpha, Ray::VIEW);
					return result += krefract.cwiseProduct(computeShading(refRay, epsilon, INF, s, area, recurs, refrac));
				}
			}
			recurs = recurs - 1 ;
			double index = m->n;
			double R = schlicks(to/current, c1);
			Ray v0(p, reflect, vray.ref, vray.alpha, Ray::VIEW);
			Ray v1(p, t, temp_ref, temp_alpha, Ray::VIEW);
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
				result += krefract.cwiseProduct((1 - R) * computeShading(v1, epsilon, INF, s, area, recurs, refrac));
			}
		} else if (m->ki != Vector3d::Zero()) {
			d = vray.dir;
			Ray refRay(p, d - 2 * (d.dot(n)) * n, vray.ref, vray.alpha, Ray::VIEW);
			result += m->ki.cwiseProduct(computeShading(refRay, epsilon, INF, s, area, recurs - 1, refrac));
		}
		return result;
	}
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