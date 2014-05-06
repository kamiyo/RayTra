/*
 * Box.h
 *
 *  Created on: Dec 18, 2011
 *      Author: kamiyo
 */

#ifndef BOX_H_
#define BOX_H_

#include "UtilDefines.h"
class Box {
public:
	Box();
	virtual ~Box();
	Vector3d b[2];
};

#endif /* BOX_H_ */
