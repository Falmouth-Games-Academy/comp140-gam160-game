#include "Player.h"
#include "Object.h"



Player::Player()
{
	
	

}

void Player::Update()
{
	Object::Update();

	if (swtch2 == 0)
	{
		if (thisRot1 > lastRot1)
		{
			ypos = ypos + 5;
		}

		if (thisRot1 < lastRot1)
		{
			ypos = ypos - 5;
		}

		lastRot1 = serialInterface->getRot1();
	}

	if (swtch2 == 1)
	{
		if (thisRot1 > lastRot1)
		{
			xpos = xpos + 5;
		}

		if (thisRot1 < lastRot1)
		{
			xpos = xpos - 5;
		}

		lastRot1 = serialInterface->getRot1();
	}

	if (swtch1 == 1)
	{
		ypos = ypos + 30;

	}
}

Player::~Player()
{
}

