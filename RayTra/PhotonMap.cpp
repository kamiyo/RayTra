#include "PhotonMap.h"

struct cmp {
	cmp(int axis) { this->m_axis = axis; }
	bool operator()(const Photon& a, const Photon& b) {
		return a.m_eye[m_axis] < b.m_eye[m_axis];
	}
	int m_axis;
};

PhotonMap::PhotonMap(const u_ptr<Photons>& photons)
{
	size_t N = photons->size();
	if (N == 1) {
		m_photon = photons->pop_back();
		m_left = nullptr;
		m_right = nullptr;
		return;
	}
	if (N == 2) {
		m_photon = photons->pop_back();
		m_left = std::make_unique<PhotonMap>(photons);
		m_right = nullptr;
		return;
	}
	std::vector<Photon>& s = photons->m_photons;
	BBox b = photons->m_bb;
	Vector3d diff = b.max() - b.min();
	int axis;
	double range = diff.maxCoeff(&axis);
	std::nth_element(s.begin(), s.begin() + s.size() / 2, s.end(), cmp(axis));
	u_ptr<Photons> left = std::make_unique<Photons>();
	u_ptr<Photons> right = std::make_unique<Photons>();
	m_photon = s[N / 2];
	double m = m_photon.m_eye[axis];
	for (size_t i = 0; i < N; i++) {
		if (s[i].m_eye[axis] < m) {
			left->push_back(s[i]);
		}
		else {
			right->push_back(s[i]);
		}
	}
	m_left = std::make_unique<PhotonMap>(left);
	m_right = std::make_unique<PhotonMap>(right);
}


PhotonMap::~PhotonMap()
{
}
