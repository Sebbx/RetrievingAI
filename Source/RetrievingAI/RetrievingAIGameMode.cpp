// Copyright Epic Games, Inc. All Rights Reserved.

#include "RetrievingAIGameMode.h"
#include "RetrievingAICharacter.h"
#include "UObject/ConstructorHelpers.h"

ARetrievingAIGameMode::ARetrievingAIGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
