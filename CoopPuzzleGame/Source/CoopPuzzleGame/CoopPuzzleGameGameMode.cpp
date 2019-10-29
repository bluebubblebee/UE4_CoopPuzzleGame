// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "CoopPuzzleGameGameMode.h"
#include "CoopPuzzleGamePlayerController.h"
#include "CoopPuzzleGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

#include "Kismet/GameplayStatics.h"
#include "Interactives/BasicInteractive.h"


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

void ACoopPuzzleGameGameMode::BeginPlay()
{
	Super::BeginPlay();

	GetInteractivesInLevel();
}


void ACoopPuzzleGameGameMode::GetInteractivesInLevel()
{
	// Find all objects of basic interactive type
	TArray<AActor*> Objects;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABasicInteractive::StaticClass(), Objects);
	for (int32 i = 0; i < Objects.Num(); i++)
	{
		ABasicInteractive* Interactive = Cast<ABasicInteractive>(Objects[i]);
		if (Interactive != nullptr)
		{

			UE_LOG(LogTemp, Warning, TEXT("[ARoomGameMode::GetInteractivesInRoom] Interactive: %s "), *Interactive->GetName());

			InteractiveInLevelList.Add(Interactive);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("[ARoomGameMode::GetInteractablesInRoom] InteractableList Num: %i "), InteractiveInLevelList.Num());
}

ABasicInteractive* ACoopPuzzleGameGameMode::FindInteractiveById(const FName& ID) const
{
	for (int32 i = 0; i < InteractiveInLevelList.Num(); i++)
	{
		if (InteractiveInLevelList[i]->ID == ID)
		{
			UE_LOG(LogTemp, Warning, TEXT("[ARoomGameMode::FindInteractiveById] Interactive: %s "), *InteractiveInLevelList[i]->GetName());

			return InteractiveInLevelList[i];
		}
	}

	return nullptr;
}