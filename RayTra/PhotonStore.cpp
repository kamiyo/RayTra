#include "PhotonStore.h"


PhotonStore::PhotonStore() {
}

PhotonStore::PhotonStore(Vector3d pos, Vector3d dir, Vector3d normal, w_ptr<Light> light, int flag) : m_eye(pos), m_dir(dir), m_flag(flag), m_light(light), m_normal(normal) {
}

PhotonStore::PhotonStore(Vector3d pos, Vector3d dir, Vector3d normal, Vector3d intensities, w_ptr<Light> light, int flag) : m_eye(pos), m_dir(dir), m_normal(normal), m_intensities(intensities), m_flag(flag), m_light(light)
{
}

void PhotonStore::set(Vector3d pos, Vector3d dir, Vector3d normal, w_ptr<Light> light, int flag) {
	m_eye = pos;
	m_dir = dir;
	m_light = light;
	m_flag = flag;
	m_normal = normal;
}

void PhotonStore::drawPhoton(std::vector<float>& vertices, std::vector<float>& colors, int flag) const {
	if ((flag < 2 && m_flag == flag) || (flag == 3 && m_flag < 2)) {
		for (int i = 0; i < 3; i++) {
			Vector3d temp = m_intensities.normalized();
			colors.push_back(temp[i]);
		}
		vertices.push_back(m_eye[0]); vertices.push_back(m_eye[1]); vertices.push_back(m_eye[2]);
	}
	else if (flag == 2 && m_flag == flag) {
		for (int i = 0; i < 3; i++) {
			colors.push_back(1);
		}
		vertices.push_back(m_eye[0]); vertices.push_back(m_eye[1]); vertices.push_back(m_eye[2]);
	}
}

PhotonStore::~PhotonStore()
{
}


Photons::Photons(int size) {
	m_photons = std::vector<u_ptr<PhotonStore> >();
	m_photons.reserve(size);
	m_bb = BBox();
}

void Photons::set(u_ptr<PhotonStore>& p, int i) {
	if (i > size() - 1) {
		m_photons.resize(i + 1);
	}
	m_photons[i] = std::move(p);
}

void Photons::setNull(int i) {
	if (i > size() - 1) {
		m_photons.resize(i + 1);
	}
	m_photons[i] = nullptr;
}

void Photons::resize(int i) {
	m_photons.resize(i);
}


void Photons::push_back() {
	m_photons.push_back(nullptr);
}

void Photons::push_back(PhotonStore p) {
	m_photons.push_back(std::make_unique<PhotonStore>(p));
	m_bb = m_bb.combine(BBox(p.m_eye, p.m_eye));
}

void Photons::insert(u_ptr<Photons>& p) {
	m_photons.reserve(p->m_photons.size() + m_photons.size());
	while (p->size() != 0) {
		push_back(p->m_photons[p->size() - 1]);
		p->m_photons.pop_back();
	}
}


void Photons::push_back(u_ptr<PhotonStore>& p) {
	m_bb = m_bb.combine(BBox(p->m_eye, p->m_eye));
	m_photons.push_back(std::move(p));
}

u_ptr<PhotonStore> Photons::pop_back() {
	u_ptr<PhotonStore> temp = std::move(m_photons.back());
	m_photons.pop_back();
	return temp;
}

int Photons::size() const {
	return m_photons.size();
}
