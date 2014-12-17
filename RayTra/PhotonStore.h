#ifndef PHOTON_STORE_H_
#define PHOTON_STORE_H_
#include "RayBase.h"
#include "Light.h"

class PhotonStore
{
public:
	PhotonStore(); PhotonStore::PhotonStore(Vector3d pos, Vector3d dir, Vector3d normal, w_ptr<Light> light, int flag);
	PhotonStore(Vector3d pos, Vector3d dir, Vector3d intensities, Vector3d normal, w_ptr<Light> light, int flag);
	void set(Vector3d pos, Vector3d dir, Vector3d normal, w_ptr<Light> light, int flag = DIRECT);
	void drawPhoton(std::vector<float>& vertices, std::vector<float>& colors, int flag) const;
	~PhotonStore();
	Vector3d m_eye, m_dir, m_normal;
	Vector3d m_intensities, m_irradiance;
	int m_flag;
	w_ptr<Light> m_light;
	static const int DIRECT = 0, INDIRECT = 1, SHADOW = 2;
};

class Photons {
public:
	Photons(int size = 0);
	void push_back();
	void push_back(PhotonStore p);
	void push_back(u_ptr<PhotonStore>& p);
	void insert(u_ptr<Photons>& p);
	void resize(int i);
	void set(u_ptr<PhotonStore>& p, int i);
	void setNull(int i);
	u_ptr<PhotonStore>& operator[](int i) { return m_photons[i]; }
	u_ptr<PhotonStore> pop_back();
	int size() const;
	std::vector<u_ptr<PhotonStore> > m_photons;
	BBox m_bb;
};

#endif