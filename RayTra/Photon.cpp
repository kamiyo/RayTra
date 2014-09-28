#include "Photon.h"


Photon::Photon(Vector3d pos, Vector3d dir, double intensity, int color) : RayBase(pos, dir, RayBase::PHOTON), m_intensity(intensity), m_color(color)
{
	Photon::count++;
}

Photon::Photon() {}


Photon::~Photon()
{
}
