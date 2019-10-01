// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "CoopPuzzleGameGameMode.h"
#include "CoopPuzzleGamePlayerController.h"
#include "CoopPuzzleGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACoopPuzzleGameGameMode::ACoopPuzzleGameGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ACoopPuzzleGamePlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}