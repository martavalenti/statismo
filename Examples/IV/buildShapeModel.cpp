/*
 * This file is part of the statismo library.
 *
 * Author: Marcel Luethi (marcel.luethi@unibas.ch)
 *
 * Copyright (c) 2011 University of Basel
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



#include <sys/types.h>
#include <errno.h>
#include <iostream>
#include "OpenInventorRepresenter.h"

#include "statismo/PCAModelBuilder.h"
#include "statismo/StatisticalModel.h"
#include "statismo/DataManager.h"

#include <memory>
using namespace statismo;
using std::auto_ptr;
/*
 * This example shows the ITK Wrapping of statismo can be used to build a shape model.
 */


/*function... might want it in some class?*/
/*int getdir (std::string dir, std::vector<std::string> &files, const std::string& extension=".*")
{
	itk::Directory::Pointer directory = itk::Directory::New();
	directory->Load(dir.c_str());

	for (unsigned i = 0; i < directory->GetNumberOfFiles(); i++) {
		const char* filename = directory->GetFile(i);
		if (extension == ".*" || std::string(filename).find(extension) != std::string::npos)
            files.push_back(filename);
	}

    return 0;
}*/




int main(int argc, char** argv) {

	if (argc < 3) {
		std::cout << "Usage " << argv[0] << " datadir modelname" << std::endl;
		exit(-1);
	}
	std::string datadir(argv[1]);
	std::string modelname(argv[2]);


	// All the statismo classes have to be parameterized with the RepresenterType.
	// For building a shape model with vtk, we use the vtkPolyDataRepresenter.
	const unsigned Dimensions = 3;
	typedef OpenInventorRepresenter::DatasetPointerType MeshType;
	typedef OpenInventorRepresenter RepresenterType;
	
	typedef DataManager<RepresenterType> DataManagerType;
	typedef PCAModelBuilder<RepresenterType> ModelBuilderType;
	typedef StatisticalModel<RepresenterType> StatisticalModelType;

	try {

		// We create a new representer object. For the vtkPolyDataRepresenter, we have to set a reference
		// and the alignmentType. The alignmenttype (which is here RIGID) determines how the dataset that we
		// will use will later be aligned to the reference.
		OpenInventorFile* reference = new OpenInventorFile;
		reference->ReadIVFile(datadir +"/result000.iv");

		//auto_ptr<RepresenterType> representer(RepresenterType::Create(reference, RepresenterType::RIGID));
		auto_ptr<RepresenterType> representer(RepresenterType::Create(reference));

		// We create a datamanager and provide it with a pointer  to the representer
		auto_ptr<DataManagerType> dataManager(DataManagerType::Create(representer.get()));


		// Now we add our data to the data manager
		// load the data and add it to the data manager. We take the first 17 hand shapes that we find in the data folder
		for (unsigned i = 0; i < 10; i++) {

			std::ostringstream ss;
			ss << datadir +"/result00" << i << ".iv";
			const std::string datasetFilename = ss.str();
			OpenInventorFile* dataset = new OpenInventorFile;
			dataset->ReadIVFile(datasetFilename);

			// We provide the filename as a second argument.
			// It will be written as metadata, and allows us to more easily figure out what we did later.
			dataManager->AddDataset(dataset, datasetFilename);

			// it is save to delete the dataset after it was added, as the datamanager direclty copies it.
			//dataset->Delete();
//			delete dataset;
		}

		// To actually build a model, we need to create a model builder object.
		// Calling the build model with a list of samples from the data manager, returns a new model.
		// The second parameter to BuildNewModel is the variance of the noise on our data
		auto_ptr<ModelBuilderType> modelBuilder(ModelBuilderType::Create());

		auto_ptr<StatisticalModelType> model(modelBuilder->BuildNewModel(dataManager->GetSampleDataStructure(), 0.01));

		// Once we have built the model, we can save it to disk.
		model->Save(modelname);
		std::cout << "Successfully saved shape model as " << modelname << std::endl;

		delete reference;
//		reference->Delete();
	}
	catch (StatisticalModelException& e) {
		std::cout << "Exception occured while building the shape model" << std::endl;
		std::cout << e.what() << std::endl;
	}
}
