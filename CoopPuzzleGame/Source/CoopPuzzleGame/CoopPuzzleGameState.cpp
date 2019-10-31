// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopPuzzleGameState.h"
#include "CoopPuzzleGamePlayerController.h"

void ACoopPuzzleGameState::MulticastOnRoomCompleted_Implementation(APawn* InstigatorPawn, bool bSuccess)
{
	UE_LOG(LogTemp, Warning, TEXT("[MulticastOnRoomCompleted_Implementation] CALLED"));

	// Loop through all the pawns available in the world and disable input
	/*for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; It++)
	{
		APawn* Pawn = It->Get();
		// Check when we are the controller of the pawn
		if (Pawn != nullptr && Pawn->IsLocallyControlled())
		{
			UE_LOG(LogTemp, Warning, TEXT("[AEscapeRoomGameState::MulticastOnRoomCompleted] Disabled Input on %s"), *Pawn->GetName());

			Pawn->DisableInput(nullptr);
		}
	}*/

	// Iterates on all player controllers available and calls OnRoomCompleted
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
	{
		ACoopPuzzleGamePlayerController* PC = Cast<ACoopPuzzleGamePlayerController>(It->Get());

		// Only when we really are the player controller then we can call the method, if it's the server it will only calls 1
		if (PC && PC->IsLocalController())
		{
			UE_LOG(LogTemp, Warning, TEXT("[MulticastOnRoomCompleted_Implementation] CALLED %s"), *PC->GetName());

			// Calls the method, and this will call the event, so the player controller can show the UI
			PC->OnRoomCompleted(InstigatorPawn, bSuccess);

			PC->LockInput();

			/*APawn* Pawn = PC->GetPawn();
			if (Pawn)
			{
				Pawn->DisableInput(PC);
			}*/
		}
	}
}