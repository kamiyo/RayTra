#include "PhotonMap.h"
#include <algorithm>

int leftChild(int i) {
	return 2 * i + 1;
}

int rightChild(int i) {
	return 2 * i + 2;
}

/*void nth(std::vector<u_ptr<PhotonStore> >& s, int index, int start, int end, int axis) {
	if (end - start == 1) return;
	if (end >= s.size()) end = s.size() - 1;
	
	for (int i = start; i < end; i += 5) {
		int n = 0;
		int iterStop = (i + 4 >= (end - start)) ? (end - start - 1) % 5 : 4;
		while (n != iterStop) {
			for (int j = iterStop; j > n; j--) {
				if (s[i + j]->m_eye[axis] < s[i + j - 1]->m_eye[axis]) {
					std::swap(s[i + j], s[i + j - 1]);
					n = j;
				}
			}
		}
		std::swap(s[i / 5], s[i + 2]);
	}
	nth(s, (end - start) / 10, 0, (end - start) / 5, axis);
}*/

int nextPow2(int n) {
	n--;
	n |= n >> 1;
	n |= n >> 2;
	n |= n >> 4;
	n |= n >> 8;
	n |= n >> 16;
	n++;
	return n;
}

bool isPow2(const int num) {
	if (num <= 0) return false;
	return !(num & (num - 1));
}

int findPivot(const int N) {
	int n = N / 2;
	int n1 = n + 1;
	int Nn = N - n;
	if (!isPow2(n1) && !isPow2(Nn)) {
		int highLeft = nextPow2(n1) - n1;
		int lowRight = (Nn) - nextPow2(Nn) / 2;
		n += ((highLeft > lowRight) ? lowRight : highLeft);
	}
	return n;
}

void PhotonMap::makeHeap(u_ptr<Photons>& photons, const int index) {
	int N = photons->m_photons.size();
	if (N == 1) {
		m_axes[index] = 0;
		m_storage->set(photons->m_photons[0], index);
		return;
	}
	std::vector<u_ptr<PhotonStore> >& s = photons->m_photons;
	BBox b = photons->m_bb;
	Vector3d diff = b.max() - b.min();
	double range = diff.maxCoeff(&m_axes[index]);
	int pivot = findPivot(N);
	std::nth_element(s.begin(), s.begin() + pivot, s.end(), cmpAxis(m_axes[index]));
	double m = s[pivot]->m_eye[m_axes[index]];
	m_storage->set(s[pivot], index);
	u_ptr<Photons> left = std::make_unique<Photons>();
	u_ptr<Photons> right = std::make_unique<Photons>();
	std::swap(s[pivot], s[N - 1]);	
	s.pop_back();  // not sure if faster than erase. Don't need to preserve order after begin+n/2
	//N = s.size();
	for (int i = 0; i < N - 1; i++) { // N-1 because we removed 1 element with pop_back()
		if (i < pivot) {
			left->push_back(s[i]);
		}
		else {
			right->push_back(s[i]);
		}
	}
	int leftIndex = leftChild(index);
	int rightIndex = rightChild(index);
	makeHeap(left, leftIndex);
	if (right->size() == 0) {
		m_storage->setNull(rightIndex);
	}
	else {
		makeHeap(right, rightIndex);
	}
}

PhotonMap::PhotonMap(u_ptr<Photons>& photons) {
	int n = photons->size();
	n = nextPow2(n);
	m_storage = std::make_unique<Photons>(n);
	std::cout << m_storage->m_photons.capacity() << std::endl;
	m_axes = std::vector<int>(n);
	makeHeap(photons, 0);
	int lastElement = m_storage->size() - 1;
	std::cout << m_storage->size() << std::endl;
	for (int i = m_storage->size() - 1; i >= 0; i--) {
		if (((*m_storage)[i]) != nullptr) {
			break;
		}
		lastElement = i;
	}
	m_storage->resize(lastElement + 1);
	std::cout << m_storage->size() << std::endl;
}
//{
//	int N = photons->m_photons.size();
//	if (N == 1) {
//		m_axis = 0;
//		m_photon = photons->pop_back();
//		m_left = nullptr;
//		m_right = nullptr;
//		return;
//	}
//	if (N == 2) {
//		m_axis = 0;
//		m_photon = photons->pop_back();
//		m_left = std::make_unique<PhotonMap>(photons);
//		m_right = nullptr;
//		return;
//	}
//	std::vector<PhotonStore>& s = photons->m_photons;
//	BBox b = photons->m_bb;
//	Vector3d diff = b.max() - b.min();
//	double range = diff.maxCoeff(&m_axis);
//	std::nth_element(s.begin(), s.begin() + s.size() / 2, s.end(), cmpAxis(m_axis));
//	m_photon = s[N / 2];
//
//
//	u_ptr<Photons> left = std::make_unique<Photons>();
//	u_ptr<Photons> right = std::make_unique<Photons>();
//	s.erase(s.begin() + N / 2);
//	N = s.size();
//	double m = m_photon.m_eye[m_axis];
//	for (int i = 0; i < N; i++) {
//		if (i < N / 2) {
//			left->push_back(s[i]);
//		}
//		else {
//			right->push_back(s[i]);
//		}
//	}
//	m_left = std::make_unique<PhotonMap>(left);
//	m_right = std::make_unique<PhotonMap>(right);

void PhotonMap::locatePhotons(const int index, PhotonQueue& photonQueue, const Vector3d& point, const Vector3d& normal, double& maxSqDistance, int numPhotons, int flag) const {
	if (numPhotons == 0) return;
	if (((*m_storage)[index]) == nullptr) return;
	u_ptr<PhotonStore>& photon = m_storage->m_photons[index];
	if (leftChild(index) < m_storage->size()) {
		int axis = m_axes[index];
		double delta = point[axis] - photon->m_eye[axis];
		if (delta < 0) {
			locatePhotons(leftChild(index), photonQueue, point, normal, maxSqDistance, numPhotons, flag);
			if (delta * delta < maxSqDistance) {
				if (rightChild(index) < m_storage->size())
					locatePhotons(rightChild(index), photonQueue, point, normal, maxSqDistance, numPhotons, flag);
			}
		}
		else {
			if (rightChild(index) < m_storage->size())
				locatePhotons(rightChild(index), photonQueue, point, normal, maxSqDistance, numPhotons, flag);
			if (delta * delta < maxSqDistance) {
				locatePhotons(leftChild(index), photonQueue, point, normal, maxSqDistance, numPhotons, flag);
			}
		}
	}
	// change
	if (photon->m_flag == flag || (flag == 4 && photon->m_flag < 3 )) {
		Vector3d to = photon->m_eye - point;
		if (to.normalized().dot(normal) > 0.25) return;
		if (photon->m_normal.normalized().dot(normal) < 0.75) return;
		double normSq = to.squaredNorm();
		if (normSq < maxSqDistance) {
			photonQueue.push(std::make_pair(PhotonStore(*photon), normSq));
			if (photonQueue.size() > numPhotons) {
				photonQueue.pop();
				maxSqDistance = photonQueue.top().second;
			}
		}
	}
}

void PhotonMap::precomputeIrradiance(int index, double distance, double numPhotons) {
	if (leftChild(index) < m_storage->size()) {
		precomputeIrradiance(leftChild(index), distance, numPhotons);
	}
	if (rightChild(index) < m_storage->size()) {
		precomputeIrradiance(rightChild(index), distance, numPhotons);

	}
	// 0 = direct
	// 1 = indirect
	// 2 = caustic
	// 3 = shadow
	// 4 = d + i + c
	if (((*m_storage)[index]) != nullptr) {
		PhotonQueue photons;
		Vector3d result = Vector3d::Zero();
		double sqDistance = distance;
		locatePhotons(0, photons, ((*m_storage)[index])->m_eye, Vector3d::Zero(), sqDistance, numPhotons, 3);
		while (photons.size() != 0) {
			Vector3d irradiance = photons.top().first.m_intensities;
			result += irradiance;
			photons.pop();
		}
		if (sqDistance != 0)
			result /= (M_PI * sqDistance);
		((*m_storage)[index])->m_irradiance = result;
	}
}

void PhotonMap::drawPhotons(const int index, std::vector<float>& vertices, std::vector<float>& colors, int flag) const {
	if (leftChild(index) < m_storage->size()) {
		drawPhotons(leftChild(index), vertices, colors, flag);
	}
	if (rightChild(index) < m_storage->size()) {
		drawPhotons(rightChild(index), vertices, colors, flag);
	}
	// 0 = direct
	// 1 = indirect
	// 2 = caustic
	// 3 = shadow
	// 4 = d + i + c
	if (((*m_storage)[index]) != nullptr)
		(*m_storage)[index]->drawPhoton(vertices, colors, flag);
}


PhotonMap::~PhotonMap()
{
}
