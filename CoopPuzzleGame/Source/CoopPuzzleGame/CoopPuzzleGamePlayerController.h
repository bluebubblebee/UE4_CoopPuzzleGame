// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CoopPuzzleGamePlayerController.generated.h"

UCLASS()
class ACoopPuzzleGamePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACoopPuzzleGamePlayerController();

	virtual void BeginPlay() override;

protected:

	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

protected:

	// Reference to the pawn
	class ACoopPuzzleGameCharacter* myCharacter;

	///// Input Actions //////

	UFUNCTION(BlueprintCallable, Category = "Input")
	void MoveForward(float Value);

	UFUNCTION(BlueprintCallable, Category = "Input")
	void MoveRight(float Value);

	///// Input Actions //////
};


