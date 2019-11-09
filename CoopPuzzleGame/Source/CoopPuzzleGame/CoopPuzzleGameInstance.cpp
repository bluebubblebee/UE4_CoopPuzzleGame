// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopPuzzleGameInstance.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystemTypes.h"


const static FName SESSION_NAME = TEXT("CoopPuzzleGameSession");
const static FName SERVER_NAME_SETTINGS_KEY = TEXT("ServerName");

UCoopPuzzleGameInstance::UCoopPuzzleGameInstance(const FObjectInitializer & ObjectInitializer)
{

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


void UCoopPuzzleGameInstance::CreateSession()
{
	UE_LOG(LogTemp, Warning, TEXT("[UCoopPuzzleGameInstance::CreateSession] Creating %s"), *SESSION_NAME.ToString());
}


void UCoopPuzzleGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{

}

void UCoopPuzzleGameInstance::OnFindSessionsComplete(bool Success)
{

}

void UCoopPuzzleGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{

}

void UCoopPuzzleGameInstance::OnJoinSessionsComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{


}



/*
void UCoopPuzzleGameInstance::Host(FString ServerName)
{

}


void UCoopPuzzleGameInstance::JoinSession(uint32 Index)
{


}

*/

