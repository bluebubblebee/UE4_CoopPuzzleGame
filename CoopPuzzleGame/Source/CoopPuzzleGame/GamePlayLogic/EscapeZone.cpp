// Fill out your copyright notice in the Description page of Project Settings.


#include "EscapeZone.h"
#include "Components/BoxComponent.h"
#include "CoopPuzzleGameCharacter.h"
#include "CoopPuzzleGamePlayerController.h"
#include "CoopPuzzleGameGameMode.h"
#include "UnrealNetwork.h"


AEscapeZone::AEscapeZone()
{
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootScene;

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	Collision->SetupAttachment(RootComponent);
	Collision->OnComponentBeginOverlap.AddDynamic(this, &AEscapeZone::HandleOverlap);

	PlayerCount = 0;

	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEscapeZone::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEscapeZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AEscapeZone::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	ACoopPuzzleGameCharacter * Pawn = Cast<ACoopPuzzleGameCharacter>(OtherActor);

	if (Pawn == nullptr)
	{
		return;
	}

	PlayerCount++;

	//UE_LOG(LogTemp, Warning, TEXT("[AEscapeZone::HandleOverlap] Character %s, %i"), *Pawn->GetName(), PlayerCount);

	if (PlayerCount >= 2)
	{

		UWorld* World = GetWorld();
		if (World == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("[AEscapeZone::HandleOverlap] Get World is null"));
			return;
		}


		ACoopPuzzleGameGameMode* GameMode = Cast<ACoopPuzzleGameGameMode>(World->GetAuthGameMode());

		if (GameMode == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("[AEscapeZone::HandleOverlap] GameMode NULL"));
			return;
		}

		if (GameMode != nullptr)
		{
			//GameMode->CompletedRoom(Pawn, true);
		}
	}

}

void AEscapeZone::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEscapeZone, PlayerCount);
}


