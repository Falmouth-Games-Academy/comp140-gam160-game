// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ArdunoGame.h"
#include "ArdunoGameGameMode.h"
#include "ArdunoGameCharacter.h"

AArdunoGameGameMode::AArdunoGameGameMode()
{
	// set default pawn class to our character
	DefaultPawnClass = AArdunoGameCharacter::StaticClass();	
}
