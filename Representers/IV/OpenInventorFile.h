#ifndef OPENINVENTORFILE_H_
#define OPENINVENTORFILE_H_

#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoShape.h>
#include <string>
#include <vector>

class OpenInventorFile{
public:

	typedef struct IVStruct {
		std::vector< SbVec3f >  m_points ;
		std::vector< std::vector< int32_t>> m_faces;
		/*SoCoordinate3		points;
		SoIndexedFaceSet	faces;*/
		unsigned int		n_points;
	};

	IVStruct mesh;

	unsigned int GetNumberOfPoints(){ return this->mesh.n_points;};
	double* GetPoint(unsigned int index);

	int ReadIVFile (std::string FileName);

	void Delete() { delete this; };

};

#endif