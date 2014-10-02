// base class for ray and photon

#ifndef RAY_BASE_H
#define RAY_BASE_H

#include "UtilDefines.h"

class RayBase {
public:
	RayBase() {}
	RayBase(Vector3d eye, Vector3d dir, int type) : m_eye(eye), m_dir(dir), m_type(type) {
		m_inv = m_dir.cwiseInverse();
		m_epsilon = 0.0001 / dir.norm();
		reSign();
	}
	void reSign() {
		m_sign = (m_inv.array() < 0).cast<int>();
	}
	Vector3d getPoint(double &t) {
		return m_eye + t * m_dir;
	}
	Vector3d m_eye;
	Vector3d m_dir;
	Vector3d m_inv;
	Vector3i m_sign;
	double m_epsilon;
	int m_type;
	static const int VIEW = 0, SHADOW = 1, PHOTON = 2;
};





#endif