// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "CoopPuzzleGameGameMode.h"
#include "CoopPuzzleGamePlayerController.h"
#include "CoopPuzzleGameCharacter.h"
#include "CoopPuzzleGameState.h"
#include "Interactives/BasicInteractive.h"

#include "UObject/ConstructorHelpers.h"

#include "Kismet/GameplayStatics.h"



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


void ACoopPuzzleGameGameMode::CompletedRoom(APawn* InstigatorPawn, bool bSuccess)
{
	if (InstigatorPawn == nullptr) return;

	UE_LOG(LogTemp, Warning, TEXT("[AMainGameMode::CompletedRoom] Called"));

	// Called EscapeRoom to change the state of the game and let everyone know the new state
	ACoopPuzzleGameState* GameState = GetGameState<ACoopPuzzleGameState>();

	if (GameState == nullptr) return;
	GameState->MulticastOnRoomCompleted(InstigatorPawn, bSuccess);

	UE_LOG(LogTemp, Warning, TEXT("[AMainGameMode::OnRoomCompleted] CALLING"));
	OnRoomCompleted(InstigatorPawn, bSuccess);

}