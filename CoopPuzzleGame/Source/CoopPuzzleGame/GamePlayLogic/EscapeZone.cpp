// Fill out your copyright notice in the Description page of Project Settings.


#include "EscapeZone.h"
#include "Components/BoxComponent.h"
#include "CoopPuzzleGameCharacter.h"
#include "CoopPuzzleGamePlayerController.h"
#include "CoopPuzzleGameGameMode.h"


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

	UE_LOG(LogTemp, Warning, TEXT("AEscapeZone::HandleOverlap"));

	ACoopPuzzleGameCharacter * Character = Cast<ACoopPuzzleGameCharacter>(OtherActor);
	if ((Character == nullptr) || (Character->GetController() == nullptr)) return;


	//ACoopPuzzleGamePlayerController* GamePC = Cast<ACoopPuzzleGamePlayerController>(Character->GetController());
	//if (GamePC == nullptr) return;

	PlayerCount++;

	UE_LOG(LogTemp, Warning, TEXT("[AEscapeZone::HandleOverlap] Character %s, %i"), *Character->GetName(), PlayerCount);

	if (OtherActor->Role == ROLE_Authority)
	{
		UE_LOG(LogTemp, Warning, TEXT("[AEscapeZone::HandleOverlap] Is Server"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[AEscapeZone::HandleOverlap] Is Server"));
	}

	if (PlayerCount >= 2)
	{
		UE_LOG(LogTemp, Warning, TEXT("[AEscapeZone::HandleOverlap] All players in zone"));

		// Get Game Mode
		UWorld* World = GetWorld();
		if (World == nullptr) return;
		ACoopPuzzleGameGameMode* GameMode = Cast<ACoopPuzzleGameGameMode>(World->GetAuthGameMode());

		if (GameMode == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("[AEscapeZone::HandleOverlap] GameMode NULL"));
			return;
		}

		UE_LOG(LogTemp, Warning, TEXT("[AEscapeZone::HandleOverlap] GameMode NOT NULL"));
		GameMode->CompletedRoom(Character, true);

	}

}


