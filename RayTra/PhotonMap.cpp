#include "PhotonMap.h"
#include <algorithm>

PhotonMap::PhotonMap(u_ptr<Photons>& photons)
{
	size_t N = photons->m_photons.size();
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
	//std::cout << N << std::endl;
	std::vector<Photon>& s = photons->m_photons;
	BBox b = photons->m_bb;
	//std::cout << "box: " << b << std::endl;
	Vector3d diff = b.max() - b.min();
	//std::cout << "diff: " << diff << std::endl;
	double range = diff.maxCoeff(&m_axis);
	//std::cout << "range: " << range << " axis: " << m_axis << std::endl;
	std::nth_element(s.begin(), s.begin() + s.size() / 2 + 1, s.end(), cmpAxis(m_axis));
	u_ptr<Photons> left = std::make_unique<Photons>();
	u_ptr<Photons> right = std::make_unique<Photons>();
	m_photon = s[N / 2];
	//std::cout << "middle: " << m_photon.m_eye << std::endl;
	s.erase(s.begin() + N / 2);
	N = s.size();
	double m = m_photon.m_eye[m_axis];
	for (size_t i = 0; i < N; i++) {
		//std::cout << s[i].m_eye << std::endl;
		if (i < N / 2) {
			left->push_back(s[i]);
		}
		else {
			right->push_back(s[i]);
		}
	}
	//std::cout << "leftsize: " << left->size() << " rightsize: " << right->size() << std::endl;
	m_left = std::make_unique<PhotonMap>(left);
	m_right = std::make_unique<PhotonMap>(right);
}

void PhotonMap::locatePhotons(PhotonQueue& photonQueue, const Vector3d& point, double& maxSqDistance, int numPhotons) const {
	if (photonQueue.size() == numPhotons) {
		return;
	}
	double delta = point[m_axis] - m_photon.m_eye[m_axis];
	if (delta < 0) {
		m_left->locatePhotons(photonQueue, point, maxSqDistance, numPhotons);
		if (delta * delta < maxSqDistance) {
			m_right->locatePhotons(photonQueue, point, maxSqDistance, numPhotons);
		}
	}
	else {
		m_right->locatePhotons(photonQueue, point, maxSqDistance, numPhotons);
		if (abs(delta) < maxSqDistance) {
			m_left->locatePhotons(photonQueue, point, maxSqDistance, numPhotons);
		}
	}

	double normSq = (m_photon.m_eye - point).squaredNorm();
	if (normSq < maxSqDistance) {
		photonQueue.push(std::make_pair(m_photon, normSq));
		maxSqDistance = photonQueue.top().second;
	}

}

void PhotonMap::drawPhotons(std::vector<float>& vertices, std::vector<float>& colors) const {
	if (m_left != nullptr) {
		m_left->drawPhotons(vertices, colors);
	}
	if (m_right != nullptr) {
		m_right->drawPhotons(vertices, colors);
	}
	colors.push_back((m_photon.m_color == Photon::RED) ? 1 : 0);
	colors.push_back((m_photon.m_color == Photon::GREEN) ? 1 : 0);
	colors.push_back((m_photon.m_color == Photon::BLUE) ? 1 : 0);
	colors.push_back((m_photon.m_color == Photon::RED) ? 1 : 0);
	colors.push_back((m_photon.m_color == Photon::GREEN) ? 1 : 0);
	colors.push_back((m_photon.m_color == Photon::BLUE) ? 1 : 0);

	Vector3d to = m_photon.m_eye + m_photon.m_dir;
	vertices.push_back(m_photon.m_eye[0]); vertices.push_back(m_photon.m_eye[1]); vertices.push_back(m_photon.m_eye[2]);
	vertices.push_back(to[0]); vertices.push_back(to[1]); vertices.push_back(to[2]);
}


PhotonMap::~PhotonMap()
{
}
