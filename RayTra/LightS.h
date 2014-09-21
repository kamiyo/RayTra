#ifndef LIGHTP_S_
#define LIGHTP_S_

#include "Light.h"
class LightS :
	public Light
{
public:
	LightS(Vector3d pos, Vector3d dir, double theta, double phi, double p, Vector3d rgb, Vector3d atten, double r);
	virtual ~LightS();
	virtual Vector3d getVector(Vector3d p);
	virtual double getFalloff(Vector3d p);
	virtual Vector3d getRanPoint();
	Vector3d _pos;
	Vector3d _dir;
	double _p;
	double _r;
	double _theta;
	double _phi;
};
#endif
