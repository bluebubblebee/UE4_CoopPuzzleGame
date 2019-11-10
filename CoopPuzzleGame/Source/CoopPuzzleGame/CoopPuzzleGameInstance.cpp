// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopPuzzleGameInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystemTypes.h"

#include "MainMenu/MainMenu.h"


const static FName SESSION_NAME = TEXT("CoopPuzzleGameSession");
const static FName SERVER_NAME_SETTINGS_KEY = TEXT("ServerName");

UCoopPuzzleGameInstance::UCoopPuzzleGameInstance(const FObjectInitializer & ObjectInitializer)
{
	// Initialize main menu
	static ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/CoopPuzzleGame/MainMenu/WBP_MainMenu"));

	if (MenuBPClass.Class != nullptr)
	{
		MenuClass = MenuBPClass.Class;
	}
}


void UCoopPuzzleGameInstance::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("[UCoopPuzzleGameInstance::Init] Init"));

	IOnlineSubsystem* SubSystem = IOnlineSubsystem::Get();

	if (SubSystem != nullptr)
	{
		SessionInterface = SubSystem->GetSessionInterface();

		//UE_LOG(LogTemp, Warning, TEXT("[UCoopPuzzleGameInstance::Init] Found session interface"));

		if (SessionInterface.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("[UEscapeRoomGameInstance::Init] SessionInterface.IsValid"));
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UCoopPuzzleGameInstance::OnCreateSessionComplete);

			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UCoopPuzzleGameInstance::OnDestroySessionComplete);

			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UCoopPuzzleGameInstance::OnFindSessionsComplete);

			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UCoopPuzzleGameInstance::OnJoinSessionsComplete);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[UCoopPuzzleGameInstance::Init] Found NO SUBSYSTEM"));
	}
}

void UCoopPuzzleGameInstance::LoadMainMenu()
{
	if(MenuClass == nullptr) return;

	MainMenu = CreateWidget<UMainMenu>(this, MenuClass);
	if (MainMenu == nullptr) return;

	MainMenu->Setup(this);

}




///// Session Handling events /////////////////// 
void UCoopPuzzleGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
	// It will not be success if there are more than one session with the same name already created
	if (!Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UCoopPuzzleGameInstance::OnCreateSessionComplete] UNSUCESS"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("[UNetTileMazeGameInstance::OnCreateSessionComplete] SUCESS SessionName: %s"), *SessionName.ToString());

	// Teardown Menu and change levels
	if (MainMenu != nullptr)
	{
		MainMenu->Teardown();
	}

	UEngine* Engine = GetEngine();

	if (Engine == nullptr) return;

	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("[OnCreateSessionComplete::Host]"));

	UE_LOG(LogTemp, Warning, TEXT("[OnCreateSessionComplete::OnCreateSessionComplete] HOST TRAVEL TO LOBBY"));

	UWorld* World = GetWorld();

	if (World == nullptr) return;

	//bUseSeamlessTravel = true;
	World->ServerTravel("/Game/CoopPuzzleGame/Maps/Level1?listen");

}




void UCoopPuzzleGameInstance::OnFindSessionsComplete(bool Success)
{

}

void UCoopPuzzleGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
	if (Success)
	{		
		UE_LOG(LogTemp, Warning, TEXT("[UCoopPuzzleGameInstance::OnDestroySessionComplete] Success "));
		CreateSession();
	}
	else
	{
		

		UE_LOG(LogTemp, Warning, TEXT("[UCoopPuzzleGameInstance::OnDestroySessionComplete] NO Success "));
	}
}

void UCoopPuzzleGameInstance::OnJoinSessionsComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{


}

///// ISessionMenuInterface /////////////////// 


///// ISessionMenuInterface /////////////////// 
void UCoopPuzzleGameInstance::Host(FString ServerName)
{
	DesiredServerName = ServerName;

	if (SessionInterface.IsValid())
	{
		// Checks for an existing session
		auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);

		if (ExistingSession != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("[UCoopPuzzleGameInstance::Host] There is an existing session about to remove the current one"));			

			SessionInterface->DestroySession(SESSION_NAME);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("[UCoopPuzzleGameInstance::Host] About to create session"));			

			// Create a new session
			CreateSession();
			
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[UCoopPuzzleGameInstance::Host] SessionInterface invalid"));
	}
}

void UCoopPuzzleGameInstance::JoinSession(uint32 Index)
{

}

void UCoopPuzzleGameInstance::EndSession()
{

}
///// ISessionMenuInterface /////////////////// 

void UCoopPuzzleGameInstance::CreateSession()
{
	UE_LOG(LogTemp, Warning, TEXT("[UCoopPuzzleGameInstance::CreateSession] Creating %s"), *SESSION_NAME.ToString());

	if (SessionInterface.IsValid())
	{
		FOnlineSessionSettings SessionSettings;

		// Switch between bIsLANMatch when using NULL subsystem
		if (IOnlineSubsystem::Get()->GetSubsystemName().ToString() == "NULL")
		{
			SessionSettings.bIsLANMatch = true;
		}
		else
		{
			SessionSettings.bIsLANMatch = false;
		}

		// Number of sessions
		SessionSettings.NumPublicConnections = 2;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.bUsesPresence = true;
		SessionSettings.Set(SERVER_NAME_SETTINGS_KEY, DesiredServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
	}
}



