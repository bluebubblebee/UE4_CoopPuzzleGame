// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "CoopPuzzleGamePlayerController.h"

#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"


/*UMainMenu::UMainMenu(const FObjectInitializer & ObjectInitializer)
{

}*/

bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success) return false;

	if (NewSessionButton == nullptr) return false;
	NewSessionButton->OnClicked.AddDynamic(this, &UMainMenu::OnNewSessionPressed);

	if (JoinSessionButton == nullptr) return false;
	JoinSessionButton->OnClicked.AddDynamic(this, &UMainMenu::OnJoinSessionPressed);

	return true;
}

void UMainMenu::Setup(ISessionMenuInterface* MenuInterface)
{
	SessionMenuInterface = MenuInterface;

	this->AddToViewport();

	UWorld* World = GetWorld();
	if (World == nullptr) return;

	ACoopPuzzleGamePlayerController* PlayerController = Cast<ACoopPuzzleGamePlayerController>(World->GetFirstPlayerController());
	if (PlayerController == nullptr)
	{
		return;
	}

	// Set the Input Mode for the Player Controller as the UI only
	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(this->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PlayerController->SetInputMode(InputModeData);
	PlayerController->LockInput();
	PlayerController->bShowMouseCursor = true;
}

void UMainMenu::Teardown()
{
	this->RemoveFromViewport();

	UWorld* World = GetWorld();
	if (World == nullptr) return;

	ACoopPuzzleGamePlayerController* PlayerController = Cast<ACoopPuzzleGamePlayerController>(World->GetFirstPlayerController());
	if (PlayerController == nullptr) return;

	// Set the Input Mode for game mode: allows only the player input / player controller to respond to user input.
	FInputModeGameOnly InputModeData;
	PlayerController->SetInputMode(InputModeData);
	PlayerController->UnLockInput();
	PlayerController->bShowMouseCursor = false;
}



void UMainMenu::OnNewSessionPressed()
{
	if (SessionMenuInterface == nullptr) return;
	SessionMenuInterface->Host("CoopPuzzleGameServer");
}

void UMainMenu::OnJoinSessionPressed()
{
	if (SessionMenuInterface == nullptr) return;
}

