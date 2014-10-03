#pragma once

#include "UtilDefines.h"
#include "Photon.h"

class PhotonMap
{
public:
	PhotonMap(Photons* photons);
	~PhotonMap();
	Photon m_photon;
	PhotonMap* m_left, *m_right;
};

