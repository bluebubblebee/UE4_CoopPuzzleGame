// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CoopPuzzleGameGameMode.generated.h"

UCLASS(minimalapi)
class ACoopPuzzleGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACoopPuzzleGameGameMode();

	class ABasicInteractive* FindInteractiveById(const FName& ID) const;

protected:

	virtual void BeginPlay() override;

private:

	// List of interactives in the current level
	TArray<class ABasicInteractive*> InteractiveInLevelList;

private:
	// Gets interactives in level
	void GetInteractivesInLevel();



public:

	void CompletedRoom(APawn* InstigatorPawn, bool bSuccess);

	// Blueprint event
	UFUNCTION(BlueprintImplementableEvent, Category = "GamePlay Mode", meta = (DisplayName = "Room Completed"))
	void OnRoomCompleted(APawn* InstigatorPawn, bool bSuccess);
};



