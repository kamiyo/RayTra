/*photon class*/

#ifndef PHOTON_H_
#define PHOTON_H_

#include "UtilDefines.h"

class Photon
{
public:
	Photon(Vector3d pos, Vector3d dir, double intensity, int color);
	Photon();
	void reSign();
	~Photon();
	Vector3d m_pos;
	Vector3d m_dir;
	Vector3d m_inv;
	Vector3i m_sign;
	double m_intensity;
	double m_epsilon;
	int m_color;
	int flag;
	enum {RED, GREEN, BLUE};
	static unsigned long count;
};
#endif