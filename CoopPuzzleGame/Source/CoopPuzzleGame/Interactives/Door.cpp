// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"
#include "UnrealNetwork.h"


void ADoor::StartInteracting(APawn* PawnInstigator)
{
	Super::StartInteracting(PawnInstigator);

	if (bIsOpen)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ADoor::StartInteracting] bIsOpen ==  we can go in"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[ADoor::StartInteracting] bIsOpen == false we can go in"));
	}
}

void ADoor::SendSignalToInteractive()
{
	if (!bIsOpen)
	{
		// We need to check if we are server or not
		if (Role < ROLE_Authority)
		{
			ServerDoActivatedAction();
		}
		else
		{
			DoActivatedAction();
		}
	}
}

void ADoor::ServerDoActivatedAction_Implementation()
{
	DoActivatedAction();
}

bool ADoor::ServerDoActivatedAction_Validate()
{
	return true;
}

void ADoor::DoActivatedAction()
{
	// Open the door and send an event
	bIsOpen = true;	
	OnDoorOpenedEvent();
}

void ADoor::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADoor, bIsOpen);
}



