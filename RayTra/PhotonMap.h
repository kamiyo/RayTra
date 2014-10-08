#pragma once

#include "UtilDefines.h"
#include "Photon.h"

class PhotonMap
{
public:
	PhotonMap(const u_ptr<Photons>& photons);
	~PhotonMap();
	Photon m_photon;
	u_ptr<PhotonMap> m_left, m_right;
};

