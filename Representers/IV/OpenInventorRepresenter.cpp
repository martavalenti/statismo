
#ifndef __OPENINVENTORREPRESENTER_CPP
#define __OPENINVENTORREPRESENTER_CPP


#include "statismo/HDF5Utils.h"
#include "statismo/utils.h"
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoShape.h>
#include <string>
#include <statismo/Domain.h>
#include <statismo/CommonTypes.h>
#include <H5Cpp.h>
#include "OpenInventorRepresenter.h"
#include "OpenInventorFile.h"

using statismo::VectorType;
using statismo::HDF5Utils;
using statismo::StatisticalModelException;

//typedef OpenInventorFile* DatasetPointerType;
//typedef const OpenInventorFile* DatasetConstPointerType;
//typedef SbVec3f PointType;

//inline
OpenInventorRepresenter::OpenInventorRepresenter(DatasetConstPointerType reference)
{
	   m_reference = new OpenInventorFile();
	   m_reference = (const_cast<DatasetPointerType>(reference));

	   // set the domain
	   DomainType::DomainPointsListType ptList;
	   for (unsigned int i = 0; i < m_reference->GetNumberOfPoints(); i++) {
//		   double* d = m_reference->GetPoint(i);
		   SbVec3f d = m_reference->mesh.m_points[i];
		   ptList.push_back(d);
	   }
	   m_domain = DomainType(ptList);
}

//inline
OpenInventorRepresenter::~OpenInventorRepresenter() 
{
	/*if (m_reference != 0) {
		m_reference->Delete();
		m_reference = 0;
	}*/
}

//inline
OpenInventorRepresenter*
OpenInventorRepresenter::Clone() const
{
	// this works since Create deep copies the reference
	OpenInventorRepresenter* clone;
	clone = this->Create(m_reference);
	return clone;
} 

//inline
OpenInventorRepresenter*
OpenInventorRepresenter::Load(const H5::CommonFG& fg) {


	std::string tmpfilename = statismo::Utils::CreateTmpName(".iv");

	HDF5Utils::getFileFromHDF5(fg, "./reference", tmpfilename.c_str());
	DatasetConstPointerType ref = ReadDataset(tmpfilename.c_str());
	std::remove(tmpfilename.c_str());

//	int alignment = static_cast<AlignmentType>(HDF5Utils::readInt(fg, "./alignment"));
	return OpenInventorRepresenter::Create(ref);

}


//inline
void
OpenInventorRepresenter::Save(const H5::CommonFG& fg) const {
	using namespace H5;

	std::string tmpfilename = statismo::Utils::CreateTmpName(".iv");


	WriteDataset(tmpfilename.c_str(), this->m_reference);

	HDF5Utils::dumpFileToHDF5(tmpfilename.c_str(), fg, "./reference" );

	std::remove(tmpfilename.c_str());
	//HDF5Utils::writeInt(fg, "./alignment", m_alignment);

}

//inline
OpenInventorRepresenter* OpenInventorRepresenter::Create(DatasetConstPointerType reference) {
		return new OpenInventorRepresenter(reference);
	}

//inline
OpenInventorRepresenter::DatasetPointerType
OpenInventorRepresenter::DatasetToSample(DatasetConstPointerType _pd, DatasetInfo* notUsed) const
{
	assert(m_reference != 0);

	OpenInventorFile* reference = const_cast<OpenInventorFile*>(this->m_reference);
	OpenInventorFile* pd = const_cast<OpenInventorFile*>(_pd);


	OpenInventorFile* alignedPd  = new OpenInventorFile;

/*	if (m_alignment != NONE) {

		vtkLandmarkTransform* transform = vtkLandmarkTransform::New();
		// we align all the dataset to the common reference

	  transform->SetSourceLandmarks(pd->GetPoints());
	  transform->SetTargetLandmarks(m_reference->GetPoints());
	  transform->SetMode(m_alignment);

	  m_pdTransform->SetInput(pd);
	  m_pdTransform->SetTransform(transform);
	  m_pdTransform->Update();

	  // we need to shallow copy the objet to make sure it does not die with the transform
	  alignedPd->ShallowCopy(m_pdTransform->GetOutput());

	  transform->Delete();

	}
	else {*/
	  // no alignment needed
	//	alignedPd->DeepCopy(pd);
//	}

	
	alignedPd = const_cast<OpenInventorFile*>(_pd);
	return alignedPd;
}

//inline
statismo::VectorType
OpenInventorRepresenter::SampleToSampleVector(DatasetConstPointerType _sample) const {
	assert(m_reference != 0);

	OpenInventorFile* sample = const_cast<OpenInventorFile*>(_sample);

	VectorType sampleVec = VectorType::Zero(m_reference->GetNumberOfPoints() * 3);
	// TODO make this more efficient using SetVoidArray of vtk
	for (unsigned i = 0 ; i < m_reference->GetNumberOfPoints(); i++) {
		for (unsigned j = 0; j < 3; j++) {
			unsigned idx = MapPointIdToInternalIdx(i, j);
			sampleVec(idx) = sample->GetPoint(i)[j];
		}
	}
	return sampleVec;
}


//inline
OpenInventorRepresenter::DatasetPointerType
OpenInventorRepresenter::SampleVectorToSample(const VectorType& sample) const
{

	assert (m_reference != 0);

	OpenInventorFile* reference = const_cast<OpenInventorFile*>(m_reference);
	OpenInventorFile* pd = new OpenInventorFile;
	//pd->DeepCopy(reference);
	pd = const_cast<OpenInventorFile*>(reference);

	std::vector<SbVec3f> points = pd->mesh.m_points;//GetPoints();
//	std::vector<SbVec3f> points;
	for (unsigned i = 0; i < reference->GetNumberOfPoints(); i++) {
		SbVec3f pt;
		for (unsigned d = 0; d < GetDimensions(); d++) {
			unsigned idx = MapPointIdToInternalIdx(i, d);
			pt[d] = sample(idx);
		}
//		points->SetPoint(i, pt.data());
		points[i] = pt;
	}

	pd->mesh.m_points = points;
	return pd;
}


//inline
OpenInventorRepresenter::ValueType
OpenInventorRepresenter::PointSampleFromSample(DatasetConstPointerType sample_, unsigned ptid) const {
	OpenInventorFile* sample = const_cast<DatasetPointerType>(sample_);
	if (ptid >= sample->GetNumberOfPoints()) {
		throw StatisticalModelException("invalid ptid provided to PointSampleFromSample");
	}
	return sample->mesh.m_points[ptid];
	//return vtkPoint(sample->GetPoints()->GetPoint(ptid));
}


//inline
statismo::VectorType 
OpenInventorRepresenter::PointSampleVectorToPointSample(const ValueType& v) const
{
	statismo::VectorType vec(GetDimensions());
	for (unsigned i = 0; i < GetDimensions(); i++) {
		vec(i) =  v[i];
	}
	return vec;
}



//inline
OpenInventorRepresenter::ValueType 
OpenInventorRepresenter::PointSampleToPointSampleVector(const statismo::VectorType& pointSample) const
{
	OpenInventorRepresenter::ValueType  value;
	for (unsigned i = 0; i < GetDimensions(); i++) {
		value[i] = pointSample(i);
	}
	return value;
}



//inline
unsigned
OpenInventorRepresenter::GetPointIdForPoint(const PointType& pt) const {
	assert (m_reference != 0);
	int index = -1;
	for (int i=0;i<m_reference->GetNumberOfPoints();i++)
	{
		if(m_reference->mesh.m_points[i]==pt)
		{ index=i;}
		else
		{ std::cout<<"Could not find corresponding point"<<std::endl; }
	}
    return index;
}

//inline
unsigned
OpenInventorRepresenter::GetNumberOfPoints() const {
	assert (m_reference != 0);

    return this->m_reference->GetNumberOfPoints();
}


//inline
OpenInventorRepresenter::DatasetPointerType
OpenInventorRepresenter::ReadDataset(const std::string& filename) {
	OpenInventorFile* pd = new OpenInventorFile;

	int result;
	result = pd->ReadIVFile(filename);
	if (result!=0)
	{
		throw StatisticalModelException((std::string("Could not read file ") + filename).c_str());
	}

    return pd;
}

//inline
void OpenInventorRepresenter::WriteDataset(const std::string& filename,DatasetConstPointerType pd) {

	OpenInventorFile* reference = const_cast<OpenInventorFile*>(pd);
	reference->WriteIVFile(filename);

}


//inline
void OpenInventorRepresenter::DeleteDataset(DatasetPointerType d) {
	delete d;
}

#endif // __OpenInventorRepresenter_CPP
