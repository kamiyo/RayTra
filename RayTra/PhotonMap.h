#ifndef PHOTON_MAP_H_
#define PHOTON_MAP_H_

#include "UtilDefines.h"
#include "PhotonStore.h"
#include <queue>

typedef std::pair<PhotonStore, double> PhotonPair;

struct cmpAxis {
	cmpAxis(int axis) { this->m_axis = axis; }
	bool operator()(const PhotonStore& a, const PhotonStore& b) {
		return a.m_eye[m_axis] < b.m_eye[m_axis];
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
	~PhotonMap();
	void locatePhotons(PhotonQueue& photonQueue, const Vector3d& point, double& maxSqDistance, int numPhotons, int flag) const;
	void drawPhotons(std::vector<float>& vertices, std::vector<float>& colors, int flag) const;
	PhotonStore m_photon;
	u_ptr<PhotonMap> m_left, m_right;
	int m_axis;
};

#endif