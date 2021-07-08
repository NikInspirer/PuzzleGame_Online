// Copyright Epic Games, Inc. All Rights Reserved.

#include "PuzzleGame_OnlineGameMode.h"
#include "PuzzleGame_OnlineCharacter.h"
#include "UObject/ConstructorHelpers.h"

APuzzleGame_OnlineGameMode::APuzzleGame_OnlineGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
