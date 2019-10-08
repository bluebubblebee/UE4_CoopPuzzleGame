// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicInteractive.h"
#include "CoopPuzzleGameCharacter.h"
#include "Components/BoxComponent.h"


// Sets default values
ABasicInteractive::ABasicInteractive()
{
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootScene;

	// Mesh
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainMesh"));
	Mesh->SetupAttachment(RootComponent);

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	Collision->SetupAttachment(RootComponent);

	// Dynamic events
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ABasicInteractive::BeginOverlap);
	Collision->OnComponentEndOverlap.AddDynamic(this, &ABasicInteractive::EndOverlap);

	// Mark the interactive as replicates
	SetReplicates(true);

	PrimaryActorTick.bCanEverTick = true;

}

void ABasicInteractive::BeginPlay()
{
	Super::BeginPlay();
	
}


void ABasicInteractive::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABasicInteractive::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	// Check if the interactive already have a character overlapping
	if (CharacterOverlapping != nullptr) return;

	// Cast the other actor to our character
	auto Character = Cast<ACoopPuzzleGameCharacter>(OtherActor);
	if (Character)
	{
		CharacterOverlapping = Character;

		// Detects if  the interface CharacterOverlapping
		IInteract* Interface = Cast<IInteract>(CharacterOverlapping);

		if (Interface)
		{
			SetOwner(CharacterOverlapping);

			// Notify the interface with this interactive
			Interface->NotifyInInteractRange(this);
		}
	}
}

void ABasicInteractive::EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	auto Character = Cast<ACoopPuzzleGameCharacter>(OtherActor);
	if (Character)
	{
		if ((CharacterOverlapping != nullptr) && (CharacterOverlapping == Character))
		{
			// Release the overlapped character reference
			CharacterOverlapping = nullptr;

			IInteract* Interface = Cast<IInteract>(Character);

			if (Interface)
			{
				SetOwner(nullptr);

				// Notifies the interface
				Interface->NotifyLeaveInteractRange(this);
			}
		}
	}
}


