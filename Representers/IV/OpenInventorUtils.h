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

	//typedef struct IVStruct {
		std::vector< SbVec3f >  m_points ;
		std::vector< std::vector< int32_t>> m_faces;
		/*SoCoordinate3		points;
		SoIndexedFaceSet	faces;*/
		unsigned int		n_points;
	//};

	unsigned int GetNumberOfPoints( return n_points;);

	void ReadIVFile (std::string FileName);

}

#endif