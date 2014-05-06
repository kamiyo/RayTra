#include "LightS.h"


LightS::LightS(Vector3d pos, Vector3d dir, double theta, double phi, double p, Vector3d rgb, Vector3d atten, double r) : _pos(pos), _dir(dir), _theta(theta), _phi(phi), _p(p), _r(r)
{
	_atten = atten;
	_rgb = rgb;
	_type = SPOT;
}

LightS::~LightS()
{
}

Vector3d LightS::getVector(Vector3d p) {
	return _pos - p;
}

double LightS::getFalloff(Vector3d p) {
	Vector3d l = _dir.normalized();
	
	Vector3d d = (p - _pos).normalized();
	double ld = l.dot(d);
	//std::cout << _theta << " " << _phi << " " << l << " " << d << " " << ld << std::endl;
	double cosPhiHalf = cos(_phi / 2);
	double num = ld - cosPhiHalf;
	if (num <= 0) {
		return 0.0;
	}
	double dist = (p - _pos).norm();
	double atten = 1 / (_atten[0] + (_atten[1] * dist) + (_atten[2] * dist * dist));
	if (_theta == _phi) {
		return 1.0 * atten;
	}
	double den = cos(_theta / 2) - cosPhiHalf;
	
	double res = atten * pow(num / den, _p);
	//std::cout << _p << " " << res << std::endl;
	return res;
}