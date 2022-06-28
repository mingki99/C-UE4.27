// Copyright Epic Games, Inc. All Rights Reserved.

#include "LeeDeukwooUe4GameMode.h"
#include "LeeDeukwooUe4Character.h"
#include "UObject/ConstructorHelpers.h"

ALeeDeukwooUe4GameMode::ALeeDeukwooUe4GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
