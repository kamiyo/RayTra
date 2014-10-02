/*photon class*/

#ifndef PHOTON_H_
#define PHOTON_H_

#include "UtilDefines.h"
#include "RayBase.h"

class Photon : public RayBase
{
public:
	Photon(Vector3d pos, Vector3d dir, double intensity, int color);
	Photon(Vector3d pos, Vector3d dir, double intensity, int color, std::vector<double> ref, std::vector<double> alpha);
	Photon();
	~Photon();
	void set(Vector3d pos, Vector3d dir);
	double m_intensity;
	int m_color;
	int flag;
	std::vector<double> m_refIndices;
	std::vector<double> m_atten;
	static const int RED = 0, GREEN = 1, BLUE = 2;
	static unsigned long count;
};
#endif