// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "CoopPuzzleGamePlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "CoopPuzzleGameCharacter.h"
#include "Engine/World.h"

ACoopPuzzleGamePlayerController::ACoopPuzzleGamePlayerController()
{ 
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void ACoopPuzzleGamePlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Get reference to the character controllerd by this player controller
	APawn* const pawn = GetPawn();
	if (pawn != nullptr)
	{
		myCharacter = Cast<ACoopPuzzleGameCharacter>(pawn);
	}

}

void ACoopPuzzleGamePlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
}

void ACoopPuzzleGamePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Input controller
	InputComponent->BindAxis("MoveForward", this, &ACoopPuzzleGamePlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ACoopPuzzleGamePlayerController::MoveRight);


	// Face buutton	
	InputComponent->BindAction("FaceButtonBottom", IE_Pressed, this, &ACoopPuzzleGamePlayerController::FaceButtonBottom);
}

void ACoopPuzzleGamePlayerController::MoveForward(float Value)
{
	if (Value == 0.0f) return;

	if (myCharacter != nullptr)
	{
		myCharacter->MoveForward(Value);
	}
}

void ACoopPuzzleGamePlayerController::MoveRight(float Value)
{
	if (Value == 0.0f) return;

	// The character is in charge of moving the mesh not the player controller
	if (myCharacter != nullptr)
	{
		myCharacter->MoveRight(Value);
	}
}


void ACoopPuzzleGamePlayerController::FaceButtonBottom()
{
	if (myCharacter != nullptr)
	{
		myCharacter->HandleInteractInput();
	}
}

