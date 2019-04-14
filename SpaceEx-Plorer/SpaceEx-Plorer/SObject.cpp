#include "pch.h"
#include "SObject.h"


SObject::SObject()
{
}


SObject::~SObject()
{
}

bool SObject::Init(std::string inName, std::string inVertexDataFile, std::string inColourDataFile, Renderer* inRenderer, bool InCanHaveInput=false)
{
	setName(inName);

	// Create the model
	if (!model.SetupBufferObjects(inVertexDataFile, inColourDataFile))
		return false;

	canReceiveInput = InCanHaveInput;

	// Set renderer shader for model
	inRenderer->SetShader(model);

}

void SObject::Update()
{
	// Set the position of the SObject to the median center of the 3D model
	position = model.GetCenter();
}
