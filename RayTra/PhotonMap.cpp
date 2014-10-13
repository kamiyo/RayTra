#include "PhotonMap.h"
#include <algorithm>

PhotonMap::PhotonMap(u_ptr<Photons>& photons)
{
	size_t N = photons->m_photons.size();
	if (N == 1) {
		m_axis = 0;
		m_photon = photons->pop_back();
		m_left = nullptr;
		m_right = nullptr;
		return;
	}
	if (N == 2) {
		m_axis = 0;
		m_photon = photons->pop_back();
		m_left = std::make_unique<PhotonMap>(photons);
		m_right = nullptr;
		return;
	}
	//std::cout << N << std::endl;
	std::vector<PhotonStore>& s = photons->m_photons;
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

void PhotonMap::locatePhotons(PhotonQueue& photonQueue, const Vector3d& point, double& maxSqDistance, int numPhotons, int flag) const {
	if (m_left || m_right) {
		double delta = point[m_axis] - m_photon.m_eye[m_axis];
		if (delta < 0) {
			if (m_left)
				m_left->locatePhotons(photonQueue, point, maxSqDistance, numPhotons, flag);
			if (delta * delta < maxSqDistance) {
				if (m_right)
					m_right->locatePhotons(photonQueue, point, maxSqDistance, numPhotons, flag);
			}
		}
		else {
			if (m_right)
				m_right->locatePhotons(photonQueue, point, maxSqDistance, numPhotons, flag);
			if (delta * delta < maxSqDistance) {
				if (m_left)
					m_left->locatePhotons(photonQueue, point, maxSqDistance, numPhotons, flag);
			}
		}
	}
	// change
	if (m_photon.m_flag != PhotonStore::SHADOW) {
		double normSq = (m_photon.m_eye - point).squaredNorm();
		photonQueue.push(std::make_pair(m_photon, normSq));
		if (photonQueue.size() > numPhotons) {
			photonQueue.pop();
		}
		maxSqDistance = photonQueue.top().second;
	}

}

void PhotonMap::drawPhotons(std::vector<float>& vertices, std::vector<float>& colors, int flag) const {
	if (m_left != nullptr) {
		m_left->drawPhotons(vertices, colors, flag);
	}
	if (m_right != nullptr) {
		m_right->drawPhotons(vertices, colors, flag);
	}
	// 0 = direct
	// 1 = indirect
	// 2 = shadow
	// 3 = d + i
	if ((flag < 2 && m_photon.m_flag == flag) || (flag == 3 && m_photon.m_flag < 2)) {
		colors.push_back((m_photon.m_color == PhotonStore::RED) ? 1 : 0);
		colors.push_back((m_photon.m_color == PhotonStore::GREEN) ? 1 : 0);
		colors.push_back((m_photon.m_color == PhotonStore::BLUE) ? 1 : 0);
		colors.push_back((m_photon.m_color == PhotonStore::RED) ? 1 : 0);
		colors.push_back((m_photon.m_color == PhotonStore::GREEN) ? 1 : 0);
		colors.push_back((m_photon.m_color == PhotonStore::BLUE) ? 1 : 0);
		Vector3d to = m_photon.m_eye + m_photon.m_dir;
		vertices.push_back(m_photon.m_eye[0]); vertices.push_back(m_photon.m_eye[1]); vertices.push_back(m_photon.m_eye[2]);
		vertices.push_back(to[0]); vertices.push_back(to[1]); vertices.push_back(to[2]);
	} else if (flag == 2 && m_photon.m_flag == flag) {
		colors.push_back(1);
		colors.push_back(1);
		colors.push_back(1);
		colors.push_back(1);
		colors.push_back(1);
		colors.push_back(1);
		Vector3d to = m_photon.m_eye + m_photon.m_dir;
		vertices.push_back(m_photon.m_eye[0]); vertices.push_back(m_photon.m_eye[1]); vertices.push_back(m_photon.m_eye[2]);
		vertices.push_back(to[0]); vertices.push_back(to[1]); vertices.push_back(to[2]);
	}
}


PhotonMap::~PhotonMap()
{
}
