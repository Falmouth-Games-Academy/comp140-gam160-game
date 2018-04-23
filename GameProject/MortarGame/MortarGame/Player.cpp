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
			ypos = ypos + 10;
		}

		if (thisRot1 < lastRot1)
		{
			ypos = ypos - 10;
		}

		lastRot1 = serialInterface->getRot1();
	}

	if (swtch2 == 1)
	{
		if (thisRot1 > lastRot1)
		{
			xpos = xpos + 10;
		}

		if (thisRot1 < lastRot1)
		{
			xpos = xpos - 10;
		}

		lastRot1 = serialInterface->getRot1();
	}
}

bool Player::hasFired()
{
	if (swtch1 == 1)
	{
		return true;
	}
	return false;
}

Player::~Player()
{
}

