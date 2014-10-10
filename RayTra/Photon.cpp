#include "Photon.h"

unsigned long Photon::count = 0;

Photon::Photon(Vector3d pos, Vector3d dir, double intensity, int color) : RayBase(pos, dir, RayBase::PHOTON), m_intensity(intensity), m_color(color)
{
	Photon::count++;
	m_refIndices.push_back(1.0);
	m_atten.push_back(0.0);
}

Photon::Photon(Vector3d pos, Vector3d dir, double intensity, int color, std::vector<double> ref, std::vector<double> alpha) : RayBase(pos, dir, RayBase::PHOTON), m_intensity(intensity), m_color(color), m_refIndices(ref), m_atten(alpha)
{
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

Photons::Photons() {
	m_bb = BBox();
}
void Photons::push_back(Photon p) {
	m_photons.push_back(p);
	m_bb = m_bb.combine(BBox(p.m_eye, p.m_eye));

}
Photon Photons::pop_back() {
	Photon temp = m_photons.back();
	m_photons.pop_back();
	return temp;
}
size_t Photons::size() {
	return m_photons.size();
}
