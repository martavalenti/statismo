/*
 * This file is part of the statismo library.
 *
 * Author: Marta Valenti (marta.valenti@mail.polimi.it)
 *
 * Copyright (c) 2012 University of Bern
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * Neither the name of the project's author nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */


#ifndef OPENINVENTORREPRESENTER_H_
#define OPENINVENTORREPRESENTER_H_


#include "OpenInventorFile.h"

#include <string>
#include <statismo/Domain.h>
#include <statismo/CommonTypes.h>
#include <H5Cpp.h>

class OpenInventorRepresenter{
public:

	typedef OpenInventorFile* DatasetPointerType;
	typedef const OpenInventorFile* DatasetConstPointerType;
	typedef SbVec3f PointType;

	typedef int ValueType;
	typedef statismo::Domain<PointType> DomainType;

	struct DatasetInfo {};
	static std::string GetName() { return "OpenInventorRepresenter"; }
	static unsigned GetDimensions() { return 3; }

	~OpenInventorRepresenter(); 

	static OpenInventorRepresenter* Create(DatasetConstPointerType reference);

	static OpenInventorRepresenter* Load(const H5::CommonFG& fg);

	OpenInventorRepresenter* Clone() const;

	void Delete() const { delete this; }
	static void DeleteDataset(DatasetPointerType d) ;

	unsigned GetNumberOfPoints() const;

	const statismo::Domain<PointType>& GetDomain() const;

	DatasetPointerType DatasetToSample(DatasetConstPointerType ds, DatasetInfo* notUsed) const;
	statismo::VectorType SampleToSampleVector(DatasetConstPointerType sample) const;

	DatasetPointerType SampleVectorToSample(const statismo::VectorType& sample) const;

	/*
	ValueType PointSampleFromSample(DatasetConstPointerType sample, unsigned ptid) const;
	statismo::VectorType PointSampleVectorToPointSample(const ValueType& v) const;

	ValueType PointSampleToPointSampleVector(const statismo::VectorType& pointSample) const;
	*/


	static unsigned MapPointIdToInternalIdx(unsigned ptId, unsigned componentInd) 
	{
		return ptId * GetDimensions() + componentInd;
	}

	unsigned GetPointIdForPoint(const PointType& point) const;

	void Save(const H5::CommonFG& fg) const;


private:

	
	static DatasetPointerType ReadDataset(const std::string& filename);
	static void WriteDataset(const std::string& filename, DatasetConstPointerType pd);
	

	//OpenInventorRepresenter(const std::string& reference);
	OpenInventorRepresenter(DatasetConstPointerType reference);
	//OpenInventorRepresenter(const OpenInventorRepresenter& orig);
	//OpenInventorRepresenter& operator=(const OpenInventorRepresenter& rhs);
	

	DatasetPointerType m_reference;

	DomainType m_domain;
};


#endif /* OPENINVENTORREPRESENTER_H_ */




