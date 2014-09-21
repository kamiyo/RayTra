/*photon class*/

#ifndef PHOTON_H_
#define PHOTON_H_

#include "UtilDefines.h"

class Photon
{
public:
	Photon(Vector3d pos, Vector3d dir, double intensity, int color);
	~Photon();
	Vector3d m_pos;
	Vector3d m_dir;
	double m_intensity;
	int m_color;
	int flag;
	enum {RED, GREEN, BLUE};
};
#endif