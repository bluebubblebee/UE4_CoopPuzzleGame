// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"
#include "UnrealNetwork.h"

ADoor::ADoor()
{
	ObjectToToggle = CreateDefaultSubobject<USceneComponent>(TEXT("ObjectToToggle"));
	ObjectToToggle->SetupAttachment(RootComponent);
}


void ADoor::BeginPlay()
{
	Super::BeginPlay();

	if (ObjectToToggle == nullptr) return;

	ObjectToToggle->SetVisibility(false, true);
}


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
	UE_LOG(LogTemp, Warning, TEXT("[ADoor::SendSignalToInteractive] "));

	Super::SendSignalToInteractive();

	if (!bIsOpen)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ADoor::SendSignalToInteractive] !bIsOpen"));

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
	UE_LOG(LogTemp, Warning, TEXT("[ADoor::ServerDoActivatedAction_Implementation]"));

	DoActivatedAction();
}

bool ADoor::ServerDoActivatedAction_Validate()
{
	return true;
}

void ADoor::DoActivatedAction()
{
	// Open the door and send an event

	UE_LOG(LogTemp, Warning, TEXT("[ADoor::DoActivatedAction] "));

	bIsOpen = true;	
	OnDoorOpenedEvent();
	OnRep_IsOpenChanged();
}


void ADoor::OnRep_IsOpenChanged()
{
	if (bIsOpen)
	{
		ObjectToToggle->SetVisibility(true, true);
	}
	else
	{
		ObjectToToggle->SetVisibility(false, true);

	}
}


void ADoor::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADoor, bIsOpen);
}



