#include "PhotonStore.h"


PhotonStore::PhotonStore() {
}

PhotonStore::PhotonStore(Vector3d pos, Vector3d dir, w_ptr<Light> light, int flag) : RayBase(pos, dir, RayBase::PHOTON), m_flag(flag), m_light(light){
}

PhotonStore::PhotonStore(Vector3d pos, Vector3d dir, double intensity, int color, w_ptr<Light> light, int flag) : RayBase(pos, dir, RayBase::PHOTON), m_intensity(intensity), m_color(color), m_flag(flag), m_light(light)
{
}

void PhotonStore::set(Vector3d pos, Vector3d dir, w_ptr<Light> light, int flag) {
	m_eye = pos;
	m_dir = dir;
	m_light = light;
	m_flag = flag;
}

PhotonStore::~PhotonStore()
{
}

Photons::Photons() {
	m_bb = BBox();
}

void Photons::push_back(PhotonStore p) {
	m_photons.push_back(p);
	m_bb = m_bb.combine(BBox(p.m_eye, p.m_eye));
}

PhotonStore Photons::pop_back() {
	PhotonStore temp = m_photons.back();
	m_photons.pop_back();
	return temp;
}

size_t Photons::size() {
	return m_photons.size();
}
