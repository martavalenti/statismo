CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(ssm_iv)
SET(CMAKE_VERBOSE_MAKEFILE ON)

FIND_PACKAGE(Statismo REQUIRED)

FIND_PATH(COIN coin)
    
FILE(GLOB INCLUDE_FILES *.h)
FILE(GLOB SOURCE_FILES  *.txx *.cpp )


include_directories(${STATISMO_DIR} ${STATISMO_DIR}/3rdParty  ${STATISMO_INCLUDE_DIRS}  
					${STATISMO_DIR}/include/Representers/IV
					${HDF5_INCLUDE_DIR} ${HDF5_INCLUDE_DIR_CPP}	
					${COIN}/include
					)



link_directories( ${HDF5_LIBRARY_DIR}  ${STATISMO_LIBRARY_DIR} ${COIN}/lib)


add_executable (ivBuildShapeModel buildShapeModel.cpp 
	${STATISMO_DIR}/include/Representers/IV/OpenInventorFile.cpp 
	${STATISMO_DIR}/include/Representers/IV/OpenInventorRepresenter.cpp
	${SOURCE_FILES}
	${INCLUDE_FILES}
) 
ADD_DEPENDENCIES(ivBuildShapeModel HDF5)
target_link_libraries (ivBuildShapeModel 
			debug coin3d 
			debug hdf5d 
			debug hdf5_cppd)

target_link_libraries (ivBuildShapeModel 
			optimized coin3
			optimized hdf5 
			optimized hdf5_cpp)

add_executable (ivSamplingExample BasicSamplingExample.cxx 
	${STATISMO_DIR}/include/Representers/IV/OpenInventorFile.cpp 
	${STATISMO_DIR}/include/Representers/IV/OpenInventorRepresenter.cpp
) 
ADD_DEPENDENCIES(ivSamplingExample HDF5)
target_link_libraries (ivSamplingExample 
			debug coin3d 
			debug hdf5d 
			debug hdf5_cppd)

target_link_libraries (ivSamplingExample 
			optimized coin3
			optimized hdf5 
			optimized hdf5_cpp)

add_definitions(-DCOIN_NOT_DLL)

INSTALL(TARGETS  ivBuildShapeModel  RUNTIME DESTINATION bin)
