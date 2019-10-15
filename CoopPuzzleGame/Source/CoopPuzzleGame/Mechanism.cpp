// Fill out your copyright notice in the Description page of Project Settings.


#include "Mechanism.h"
#include "UnrealNetwork.h"

void AMechanism::StartInteracting(APawn* PawnInstigator)
{
	Super::StartInteracting(PawnInstigator);

	if (bIsActivated)
	{
		UE_LOG(LogTemp, Warning, TEXT("[AMechanism::StartInteracting] bIsActivated, we can't activated it again"));
	}
	else
	{
		// Check my role, if I am not the server-client call the server
		if (Role < ROLE_Authority)
		{
			ServerDoActivatedAction();
		}
		else
		{
			// I am the server, do the action
			DoActivatedAction();
		}
	}
}

void AMechanism::ServerDoActivatedAction_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("[AMechanism::ServerDoActivatedAction]"));

	DoActivatedAction();
}

bool AMechanism::ServerDoActivatedAction_Validate()
{
	return true;
}

void AMechanism::DoActivatedAction()
{
	UE_LOG(LogTemp, Warning, TEXT("[AMechanism::ServerDoActivatedAction]"));

	bIsActivated = true;

	// Call the event
	OnMechanismActivatedEvent();
}


void AMechanism::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMechanism, bIsActivated);
}

