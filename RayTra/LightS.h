#ifndef LIGHTP_S_
#define LIGHTP_S_

#include "Light.h"
class LightS :
	public Light
{
public:
	LightS(Vector4d pos, Vector4d dir, double theta, double phi, double p, Vector4d rgb, Vector4d atten, double r);
	virtual ~LightS();
	virtual Vector4d getVector(Vector4d p);
	virtual double getFalloff(Vector4d p);
	Vector4d _pos;
	Vector4d _dir;
	double _p;
	double _r;
	double _theta;
	double _phi;
};
#endif
