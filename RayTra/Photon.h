/*photon class*/

#ifndef PHOTON_H_
#define PHOTON_H_

#include "UtilDefines.h"
#include "RayBase.h"
#include "BBox.h"

class Light;

class Photon : public RayBase
{
public:
	Photon(Vector3d pos, Vector3d dir, double intensity, int color, w_ptr<Light> light = w_ptr<Light>());
	Photon(Vector3d pos, Vector3d dir, double intensity, int color, w_ptr<Light> light, std::vector<double> ref, std::vector<double> alpha);
	Photon();
	~Photon();
	void set(Vector3d pos, Vector3d dir, w_ptr<Light> light);
	void set(Vector3d pos, Vector3d dir);
	double m_intensity;
	int m_color;
	w_ptr<Light> m_light;
	std::vector<double> m_refIndices;
	std::vector<double> m_atten;
	static const int RED = 0, GREEN = 1, BLUE = 2;
	static unsigned long count;
};

#endif