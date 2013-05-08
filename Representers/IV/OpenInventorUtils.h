#ifndef OPENINVENTORUTILS_H_
#define OPENINVENTORUTILS_H_

#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoShape.h>
#include <string>

class OpenInventorUtils{
public:

	typedef struct IVStruct {
		SoCoordinate3		points;
		SoIndexedFaceSet	faces;
	};

	unsigned int GetNumberOfPoints();
}

#endif