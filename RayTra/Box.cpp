/*
 * Box.cpp
 *
 *  Created on: Dec 18, 2011
 *      Author: kamiyo
 */

#include "Box.h"
/*
VARS
vec		b[2]	(minX,minY,minZ) and (maxX,maxY,maxZ) of bounding box

*/

/*
Intialize to inf
*/
Box::Box() {
	MIN.setZero(); MIN << nINF, nINF, nINF;
	MAX.setZero(); MAX << INF, INF, INF;
}

Box::~Box() {
	// TODO Auto-generated destructor stub
}
