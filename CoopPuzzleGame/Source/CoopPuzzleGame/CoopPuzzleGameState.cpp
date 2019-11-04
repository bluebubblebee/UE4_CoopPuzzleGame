// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopPuzzleGameState.h"
#include "CoopPuzzleGamePlayerController.h"

void ACoopPuzzleGameState::MulticastOnRoomCompleted_Implementation(APawn* InstigatorPawn)
{
	//UE_LOG(LogTemp, Warning, TEXT("[MulticastOnRoomCompleted_Implementation] CALLED"));

	// Iterates on all player controllers available and calls OnRoomCompleted
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
	{
		ACoopPuzzleGamePlayerController* PC = Cast<ACoopPuzzleGamePlayerController>(It->Get());

		// Only when we really are the player controller then we can call the method, if it's the server it will only calls 1
		if (PC && PC->IsLocalController())
		{
			//UE_LOG(LogTemp, Warning, TEXT("[MulticastOnRoomCompleted_Implementation] CALLED %s"), *PC->GetName());

			// Calls the method, and this will call the event, so the player controller can show the UI
			PC->OnRoomCompleted(InstigatorPawn);

			PC->LockInput();
		}
	}
}