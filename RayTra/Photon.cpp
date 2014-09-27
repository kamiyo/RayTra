#include "Photon.h"


Photon::Photon(Vector3d pos, Vector3d dir, double intensity, int color) : m_pos(pos), m_dir(dir), m_intensity(intensity), m_color(color)
{
	Photon::count++;
	m_epsilon = 0.0001 / dir.norm();
	m_inv = dir.cwiseInverse();
	reSign();
}

void Photon::reSign() {
	m_sign = (m_inv.array() < 0).cast<int>();
}

Photon::Photon() {}


Photon::~Photon()
{
}
