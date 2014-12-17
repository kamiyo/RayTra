#include "Photon.h"

unsigned long Photon::count = 0;

Photon::Photon(Vector3d pos, Vector3d dir, Vector3d intensities, w_ptr<Light> light) : RayBase(pos, dir, RayBase::PHOTON), m_intensities(intensities), m_light(light)
{
	Photon::count++;
	m_refIndices.push_back(1.0);
	m_atten.push_back(0.0);
	onlySpecular = false;
}

Photon::Photon(Vector3d pos, Vector3d dir, Vector3d intensities, w_ptr<Light> light, std::vector<double> ref, std::vector<double> alpha) : RayBase(pos, dir, RayBase::PHOTON), m_intensities(intensities), m_refIndices(ref), m_atten(alpha), m_light(light)
{
	Photon::count++;
	onlySpecular = false;
}

void Photon::set(Vector3d pos, Vector3d dir, w_ptr<Light> light) {
	m_light = light;
	m_eye = pos;
	m_dir = dir;
	m_inv = m_dir.cwiseInverse();
	m_epsilon = 0.001 / dir.norm();
	reSign();
	Photon::count++;
}

void Photon::set(Vector3d pos, Vector3d dir, Vector3d intensities) {
	m_eye = pos;
	m_dir = dir;
	m_intensities = intensities;
	m_inv = m_dir.cwiseInverse();
	m_epsilon = 0.001 / dir.norm();
	reSign();
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
