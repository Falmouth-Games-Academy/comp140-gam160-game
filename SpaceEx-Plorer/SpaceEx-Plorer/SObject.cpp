#include "pch.h"
#include "SObject.h"

static int objIndex = 0;

SObject::SObject()
{
	following = false;
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

	// If a valid target has been set, move this model towards it
	if (target != nullptr && glm::length(this->getPosition() - this->target->getPosition()) > 1)
	{
		if (following)
		{
			model.Translate(-glm::normalize(this->getPosition() - target->getPosition()));
		}

	}
	else if (targetPoint != glm::vec3() && glm::length(this->getPosition() - targetPoint) > 2)
	{
		if (following)
		{
			model.Translate(-glm::normalize(this->getPosition() - targetPoint));
		}
	}
}
