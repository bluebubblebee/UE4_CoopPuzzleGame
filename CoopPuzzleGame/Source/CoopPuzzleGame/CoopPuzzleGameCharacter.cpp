// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "CoopPuzzleGameCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"

#include "Interactives/BasicInteractive.h"

ACoopPuzzleGameCharacter::ACoopPuzzleGameCharacter() 
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->RelativeRotation = FRotator(-60.f, 0.f, 0.f);
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; 

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void ACoopPuzzleGameCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}


void ACoopPuzzleGameCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ACoopPuzzleGameCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void ACoopPuzzleGameCharacter::HandleInteractInput()
{
	if (IsLocallyControlled() && (CurrentInteractive != nullptr))
	{
		UE_LOG(LogTemp, Warning, TEXT("ACoopPuzzleGameCharacter::HandleInteractInput Calling StartInteract"));

		CurrentInteractive->StartInteracting(this);
	}
}



//// INTERFACE IInteract IMPLEMENTATION ////////////////////
void ACoopPuzzleGameCharacter::NotifyInInteractRange(AActor* Interactive)
{
	if (IsLocallyControlled())
	{
		// Keeps the Interactive reference
		if ((Interactive != nullptr) && (CurrentInteractive == nullptr))
		{
			CurrentInteractive = Cast<ABasicInteractive>(Interactive);
		}
	}
}

void ACoopPuzzleGameCharacter::NotifyLeaveInteractRange(AActor* Interactive)
{
	if (IsLocallyControlled())
	{
		// Release the Interactive reference
		if (CurrentInteractive != nullptr)
		{
			CurrentInteractive = nullptr;
		}
	}
}
//// INTERFACE IInteract IMPLEMENTATION ////////////////////
