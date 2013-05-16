CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(ssm_iv)
SET(CMAKE_VERBOSE_MAKEFILE ON)

FIND_PACKAGE(Statismo REQUIRED)

FIND_PATH(COIN coin)
    

FILE(GLOB INCLUDE_FILES *.h)
FILE(GLOB SOURCE_FILES  *.txx )

include_directories(${STATISMO_DIR} ${STATISMO_DIR}/3rdParty  ${STATISMO_INCLUDE_DIRS}  
					${STATISMO_DIR}/include/Representers/IV
					${HDF5_INCLUDE_DIR} ${HDF5_INCLUDE_DIR_CPP}	
					${COIN}/include
					)
link_directories( ${HDF5_LIBRARY_DIR}  ${STATISMO_LIBRARY_DIR} ${COIN}/lib)


add_executable (itkBuildShapeModel buildShapeModel.cpp 
	${STATISMO_DIR}/include/Representers/IV/OpenInventorFile.cpp 
	${STATISMO_DIR}/include/Representers/IV/OpenInventorRepresenter.cpp

) 
ADD_DEPENDENCIES(itkBuildShapeModel HDF5)
target_link_libraries (itkBuildShapeModel 
			coin3d coin3
			hdf5 hdf5_cpp
			hdf5d hdf5_cppd)


add_definitions(-DCOIN_NOT_DLL)

INSTALL(TARGETS  itkBuildShapeModel  RUNTIME DESTINATION bin)
