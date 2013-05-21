#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor\actions\SoWriteAction.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoShape.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "OpenInventorFile.h"

#define n_pt 2562
#define n_fc 20480

OpenInventorFile::OpenInventorFile()
{
	this->mesh.n_points=0;
	this->mesh.n_faces=0;
	this->mesh.m_faces.empty();
	this->mesh.m_points.empty();
};

OpenInventorFile::~OpenInventorFile()
{}

SbVec3f OpenInventorFile::GetPoint(unsigned int index)
{
	SbVec3f result;
	result = this->mesh.m_points[index];
	return result;
}

int
OpenInventorFile::ReadIVDatasetFile(std::string FileName)
{	
	SoInput in;
	SoSeparator* soSeparator;
	
		
	if (in.openFile(FileName.c_str()) == false)
	{
		std::cout << "Error in openening file";
		return 1;
	}
	else
	{
		//soSeparator = SoDB::readAll(&in);
		this->mesh.surface = SoDB::readAll(&in);
		in.closeFile();
	}
	return 0;
}

int
OpenInventorFile::ReadIVFile(std::string FileName)
{
	std::vector< SbVec3f >  OutPoint ;
	std::vector< std::vector< int32_t>> OutFace;
	int result = 0;
	
	SoInput in;
	SoSeparator* soSeparator;
	
		
	if (in.openFile(FileName.c_str()) == false)
	{
		std::cout << "Error in openening file";
		result = 1;
		return result;
	}
	else
	{
		
		this->mesh.surface = SoDB::readAll(&in);
	}

	if (in.openFile(FileName.c_str()) == false)
	{
		std::cout << "Error in openening file";
		result = 1;
		return result;
	}
	else
	{
		soSeparator = SoDB::readAll(&in);
		in.closeFile();

		SoCoordinate3 *myCoords;
	  
      const SbVec3f *Vec_coords;
      int num_vectors;
	 
      // insert it into a vector
      std::vector< SbVec3f > SurfacePoints;
 
      SoSearchAction mySearch;
	  
      mySearch.setType(SoCoordinate3::getClassTypeId());
      mySearch.setInterest(SoSearchAction::FIRST);
      mySearch.apply(soSeparator);

      
	  //search for points
      if(mySearch.getPath()==NULL)
      {
            std::cout << "Error in reading path";
			result = 2;
			return result;
			//break;
      }
 
      else
      {
            // read all points from reference object
            myCoords  =  reinterpret_cast<SoCoordinate3 *>( mySearch.getPath()->getTail());
            if(!myCoords)
            {
                  std::cout << "Error in reading coordinates";
				  result = 3;
				  return result;
				  //break;
            }
 
            num_vectors =   (myCoords->point).getNum();
			this->mesh.n_points = num_vectors;
 
            // Get the starting point of the coordiantes
            Vec_coords =  (myCoords->point).getValues(0);
     
 
            for (int i = 0; i  < num_vectors; i++)
                  SurfacePoints.push_back (Vec_coords[i]);
      }
 
      // assign it
      OutPoint = SurfacePoints;
	  this->mesh.m_points = SurfacePoints;

	  //search for faces
	  SoIndexedFaceSet *myIndexFace;

	  const int32_t *Vec_face_coords;
  	  std::vector< int32_t> SingleFace;
	  std::vector<std::vector< int32_t >> Faces;

	  SoSearchAction myFaceSearch;

	  myFaceSearch.setType(SoIndexedFaceSet::getClassTypeId());
      myFaceSearch.setInterest(SoSearchAction::FIRST);
      myFaceSearch.apply(soSeparator);


      if(myFaceSearch.getPath()==NULL)
      {
            std::cout<< "error in reading faces path";
			result = 4;
			return result;
      }
 
      else
      {
            // read all faces from reference object
            myIndexFace  =  reinterpret_cast<SoIndexedFaceSet *>( myFaceSearch.getPath()->getTail());
            if(!myIndexFace)
            {
                  std::cout << "error in reading faces";
				  result = 5;
				  return result;
            }
 
            num_vectors =   (myIndexFace->coordIndex).getNum();
			this->mesh.n_faces = num_vectors;
 
            // Get the starting point of the coordiantes
            Vec_face_coords =  (myIndexFace->coordIndex).getValues(0);
     
 
            for (int i = 0; i  < num_vectors/4; i++)
			{
				for (int j = 0; j < 4; j++)
					SingleFace.push_back (Vec_face_coords[i*4+j]);
				Faces.push_back (SingleFace);
				SingleFace.clear();
			}
//			aMeshesElements.push_back(Faces);
			this->mesh.m_faces = (Faces);
      }
	}
	return result;
};

int
OpenInventorFile::WriteIVFile(std::string FileName)
{
	SoSeparator* myShape = new SoSeparator;
	myShape = this->shapeCreator();
	//myShape = this->mesh.surface;

	SoWriteAction myAction;

	myAction.getOutput()->openFile(FileName.c_str());
	myAction.getOutput()->setBinary(FALSE);
	myAction.apply(myShape);
	myAction.getOutput()->closeFile();
	
	return 0;
};

SoSeparator* OpenInventorFile::shapeCreator()
{

	SoSeparator *result = new SoSeparator;
	result->ref();


	// replace points on the referencee mesh
	SoCoordinate3 *myCoords = new SoCoordinate3;
	for (int i=0;i<this->mesh.n_points;i++)
	{
		myCoords->point.set1Value(i, this->mesh.m_points[i]);
	}
	result->addChild(myCoords);

	SoIndexedFaceSet* myFaceSet = new SoIndexedFaceSet;
	for (int i=0;i<this->mesh.n_faces/4;i++)
	{
		myFaceSet->coordIndex.set1Value(i*4,this->mesh.m_faces[i][0]);
		myFaceSet->coordIndex.set1Value(i*4+1,this->mesh.m_faces[i][1]);
		myFaceSet->coordIndex.set1Value(i*4+2,this->mesh.m_faces[i][2]);
		myFaceSet->coordIndex.set1Value(i*4+3,this->mesh.m_faces[i][3]);
	}
	result->addChild(myFaceSet);

	result->unrefNoDelete();
	return result;
}