#ifndef PHOTON_MAP_H_
#define PHOTON_MAP_H_

#include "UtilDefines.h"
#include "PhotonStore.h"
#include <queue>

typedef std::pair<PhotonStore, double> PhotonPair;

struct cmpAxis {
	cmpAxis(int axis) { this->m_axis = axis; }
	bool operator()(const u_ptr<PhotonStore>& a, const u_ptr<PhotonStore>& b) {
		return a->m_eye[m_axis] < b->m_eye[m_axis];
	}
	int m_axis;
};

struct cmpDist {
	bool operator()(const PhotonPair& a, const PhotonPair& b) {
		return a.second < b.second;
	}
};

typedef std::priority_queue<PhotonPair, std::vector<PhotonPair>, cmpDist> PhotonQueue;

class PhotonMap
{
public:
	PhotonMap(u_ptr<Photons>& photons);
	void makeHeap(u_ptr<Photons>& photons, const int index);
	~PhotonMap();
	void locatePhotons(int index, PhotonQueue& photonQueue, const Vector3d& point, const Vector3d& normal, double& maxSqDistance, int numPhotons, int flag) const;
	void drawPhotons(int index, std::vector<float>& vertices, std::vector<float>& colors, int flag) const;
	void precomputeIrradiance(int index, double distance, double numPhotons);
	u_ptr<Photons> m_storage;
	std::vector<int> m_axes;
};

#endif