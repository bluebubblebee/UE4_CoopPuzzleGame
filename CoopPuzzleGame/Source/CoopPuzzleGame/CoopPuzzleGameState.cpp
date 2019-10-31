// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopPuzzleGameState.h"
#include "CoopPuzzleGamePlayerController.h"

void ACoopPuzzleGameState::MulticastOnRoomCompleted_Implementation(APawn* InstigatorPawn, bool bSuccess)
{

	UE_LOG(LogTemp, Warning, TEXT("[MulticastOnRoomCompleted_Implementation] CALLED"));

	// Loop through all the player controllers available
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
	{
		ACoopPuzzleGamePlayerController* PC = Cast<ACoopPuzzleGamePlayerController>(It->Get());
		// Check when we are the owner of the controller
		if (PC && PC->IsLocalController())
		{
			UE_LOG(LogTemp, Warning, TEXT("[AEscapeRoomGameState::MulticastOnRoomCompleted] Disabled Input on %s"), *PC->GetName());

			//PC->OnRoomCompleted(InstigatorPawn, bSuccess);

			// Disable input 
			APawn* Pawn = PC->GetPawn();
			if (Pawn)
			{
				UE_LOG(LogTemp, Warning, TEXT("[AEscapeRoomGameState::MulticastOnRoomCompleted]PC->GetPawn() exist disabling input"));

				Pawn->DisableInput(PC);
			}
		}
	}
}