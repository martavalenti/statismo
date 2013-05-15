#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoShape.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "OpenInventorFile.h"

void
OpenInventorFile::ReadIVFile(std::string FileName)
{
	std::vector< SbVec3f >  OutPoint ;
	std::vector< std::vector< int32_t>> OutFace;
	
	SoInput in;
	SoSeparator* soSeparator;
		
	if (in.openFile(FileName.c_str()) == false)
	{
		std::cout << "Error in openening file";
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
			//break;
      }
 
      else
      {
            // read all points from reference object
            myCoords  =  reinterpret_cast<SoCoordinate3 *>( mySearch.getPath()->getTail());
            if(!myCoords)
            {
                  std::cout << "Error in reading coordinates";
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
      }
 
      else
      {
            // read all faces from reference object
            myIndexFace  =  reinterpret_cast<SoIndexedFaceSet *>( myFaceSearch.getPath()->getTail());
            if(!myIndexFace)
            {
                  std::cout << "error in reading faces";
            }
 
            num_vectors =   (myIndexFace->coordIndex).getNum();
 
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
	
};
	