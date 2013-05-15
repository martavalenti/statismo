CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(ssm_iv)
SET(CMAKE_VERBOSE_MAKEFILE ON)

FIND_PACKAGE(Statismo REQUIRED)
FIND_PACKAGE(ITK 4.1 REQUIRED)

include (${ITK_USE_FILE})
FIND_PATH(COIN coin)
    
SET (ITK_STATISTICAL_MODEL_INCLUDE_DIR ${STATISMO_ROOT_DIR}/ITKstatismo)

FILE(GLOB INCLUDE_FILES *.h)
FILE(GLOB SOURCE_FILES  *.txx )

include_directories(${STATISMO_DIR} ${STATISMO_DIR}/statismo_ITK  ${STATISMO_DIR}/3rdParty  ${STATISMO_INCLUDE_DIRS}  
					#${ITK_INCLUDE_DIRS}  ${STATISMO_DIR}/Representers/ITK
					${STATISMO_DIR}/include/Representers/IV
					${HDF5_INCLUDE_DIR} ${HDF5_INCLUDE_DIR_CPP}	
					${COIN}/include
					)
link_directories( ${HDF5_LIBRARY_DIR} ${ITK_LIBRARY_DIRS} ${STATISMO_LIBRARY_DIR} ${COIN}/lib)


add_executable (itkBuildShapeModel buildShapeModel.cpp 
	${STATISMO_DIR}/include/Representers/IV/OpenInventorFile.cpp 
	${STATISMO_DIR}/include/Representers/IV/OpenInventorRepresenter.cpp
	${SOURCE_FILES}
	${INCLUDE_FILES}
) 
ADD_DEPENDENCIES(itkBuildShapeModel HDF5)
target_link_libraries (itkBuildShapeModel 
			${ITK_LIBRARIES} 
			coin3d coin3)


add_definitions(-DCOIN_NOT_DLL)

INSTALL(TARGETS  itkBuildShapeModel  RUNTIME DESTINATION bin)
#INSTALL(PROGRAMS runITKExamples.sh DESTINATION  bin)
