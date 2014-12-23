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
	Photon(Vector3d pos, Vector3d dir, Vector3d intensities, w_ptr<Light> light = w_ptr<Light>());
	Photon(Vector3d pos, Vector3d dir, Vector3d intensities, w_ptr<Light> light, std::vector<double> ref, std::vector<Vector3d> alpha);
	Photon();
	~Photon();
	void set(Vector3d pos, Vector3d dir, w_ptr<Light> light);
	void set(Vector3d pos, Vector3d dir, Vector3d intensities);
	void set(Vector3d pos, Vector3d dir);
	Vector3d m_intensities;
	w_ptr<Light> m_light;
	bool onlySpecular;
	std::vector<double> m_refIndices;
	std::vector<Vector3d> m_atten;
	static unsigned long count;
};

#endif