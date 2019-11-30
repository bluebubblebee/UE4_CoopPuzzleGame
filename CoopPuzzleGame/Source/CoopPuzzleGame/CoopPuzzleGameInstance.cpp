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


void UCoopPuzzleGameInstance::OpenSessionListMenu()
{
	if (MainMenu == nullptr) return;

	SessionSearch = MakeShareable(new FOnlineSessionSearch());

	if (SessionSearch.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("[UCoopPuzzleGameInstance::OpenSessionListMenu] Session is valid"));
		//SessionSearch->bIsLanQuery = true;
		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

void UCoopPuzzleGameInstance::OnFindSessionsComplete(bool Success)
{
	if (MainMenu == nullptr) return;

	if (Success && SessionSearch.IsValid())
	{
		if (SessionSearch->SearchResults.Num() <= 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("[UCoopPuzzleGameInstance::OnFindSessionsComplete] No Sessions Find"));
		}
		else
		{
			TArray<FServerData> ServerData;
			for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
			{
				UE_LOG(LogTemp, Warning, TEXT("[UCoopPuzzleGameInstance::OnFindSessionsComplete] Session Name %s"), *SearchResult.GetSessionIdStr());

				FServerData Data;
				FString ServerName;
				if (SearchResult.Session.SessionSettings.Get(SERVER_NAME_SETTINGS_KEY, ServerName))
				{
					UE_LOG(LogTemp, Warning, TEXT("[UCoopPuzzleGameInstance::OnFindSessionsComplete] Data found in settings %s"), *ServerName);
					Data.Name = ServerName;
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("[UCoopPuzzleGameInstance::OnFindSessionsComplete] Data NOT found in settings"));

					Data.Name = "Could not find name";
				}

				Data.MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
				Data.CurrentPlayers = Data.MaxPlayers - SearchResult.Session.NumOpenPublicConnections;
				Data.HostUsername = SearchResult.Session.OwningUserName;

				ServerData.Add(Data);
			}


			MainMenu->InitializeSessionsList(ServerData);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[UCoopPuzzleGameInstance::OnFindSessionsComplete] Error session not found"));
	}
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
	if (!SessionInterface.IsValid() || (!SessionSearch.IsValid())) return;

	if (Index < (uint32)(SessionSearch->SearchResults.Num()))
	{
		SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);
	}
}

void UCoopPuzzleGameInstance::OnJoinSessionsComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	UE_LOG(LogTemp, Warning, TEXT("[UCoopPuzzleGameInstance::OnJoinSessionsComplete]"));

	if (MainMenu != nullptr)
	{
		MainMenu->Teardown();
	}
	if (!SessionInterface.IsValid()) return;

	FString Url;
	if (!SessionInterface->GetResolvedConnectString(SESSION_NAME, Url))
	{
		UE_LOG(LogTemp, Warning, TEXT("[UNetTileMazeGameInstance::OnJoinSessionsComplete] Couldn't get Connect String"));
		return;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("[UNetTileMazeGameInstance::OnJoinSessionsComplete] Url: %s"), *Url);

	APlayerController* PlayerController = GetFirstLocalPlayerController();

	if (PlayerController == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UNetTileMazeGameInstance::OnJoinSessionsComplete] Player Controller DOESN'T EXIST"));
		return;
	}


	PlayerController->ClientTravel(Url, ETravelType::TRAVEL_Absolute);

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



