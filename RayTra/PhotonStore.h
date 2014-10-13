#ifndef PHOTON_STORE_H_
#define PHOTON_STORE_H_
#include "RayBase.h"
#include "Light.h"

class PhotonStore :
	public RayBase
{
public:
	PhotonStore(); PhotonStore::PhotonStore(Vector3d pos, Vector3d dir, w_ptr<Light> light, int flag);
	PhotonStore(Vector3d pos, Vector3d dir, double intensity, int color, w_ptr<Light> light, int flag);
	void set(Vector3d pos, Vector3d dir, w_ptr<Light> light, int flag = DIRECT);
	~PhotonStore();
	double m_intensity;
	int m_color;
	int m_flag;
	w_ptr<Light> m_light;
	static const int RED = 0, GREEN = 1, BLUE = 2;
	static const int DIRECT = 0, INDIRECT = 1, SHADOW = 2;
};

class Photons {
public:
	Photons();
	void push_back(PhotonStore p);
	PhotonStore pop_back();
	size_t size();
	std::vector<PhotonStore> m_photons;
	BBox m_bb;
};

#endif