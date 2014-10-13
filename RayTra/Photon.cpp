#include "Photon.h"

unsigned long Photon::count = 0;

Photon::Photon(Vector3d pos, Vector3d dir, double intensity, int color, w_ptr<Light> light) : RayBase(pos, dir, RayBase::PHOTON), m_intensity(intensity), m_color(color), m_light(light)
{
	Photon::count++;
	m_refIndices.push_back(1.0);
	m_atten.push_back(0.0);
}

Photon::Photon(Vector3d pos, Vector3d dir, double intensity, int color, w_ptr<Light> light, std::vector<double> ref, std::vector<double> alpha) : RayBase(pos, dir, RayBase::PHOTON), m_intensity(intensity), m_color(color), m_refIndices(ref), m_atten(alpha), m_light(light)
{
	Photon::count++;
}

void Photon::set(Vector3d pos, Vector3d dir, w_ptr<Light> light) {
	m_light = light;
	m_eye = pos;
	m_dir = dir;
	Photon::count++;
}

void Photon::set(Vector3d pos, Vector3d dir) {
	m_eye = pos;
	m_dir = dir;
	Photon::count++;
}

Photon::Photon() {}

Photon::~Photon()
{
}
