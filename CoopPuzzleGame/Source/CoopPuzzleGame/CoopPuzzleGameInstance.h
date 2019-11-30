// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"


#include "OnlineSubsystem.h"
#include "OnlineSessionInterface.h"

#include "MainMenu/SessionMenuInterface.h"

#include "CoopPuzzleGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class COOPPUZZLEGAME_API UCoopPuzzleGameInstance : public UGameInstance, public ISessionMenuInterface
{
	GENERATED_BODY()

public:

	UCoopPuzzleGameInstance(const FObjectInitializer & ObjectInitializer);

	virtual void Init();

	// Create menu called from the level blueprint
	UFUNCTION(BlueprintCallable)
	void LoadMainMenu();

private:

	// Session
	IOnlineSessionPtr SessionInterface;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;


public:

	///// ISessionMenuInterface /////////////////// 
	UFUNCTION()
	void Host(FString ServerName) override;

	UFUNCTION()
	void JoinSession(uint32 Index) override;

	UFUNCTION()
	void EndSession() override;

	UFUNCTION()
	void OpenSessionListMenu() override;
	///// ISessionMenuInterface /////////////////// 



private:

	// Main Menu
	TSubclassOf<class UUserWidget> MenuClass;
	class UMainMenu* MainMenu;

private:

	// Session Events
	FString DesiredServerName;
	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void OnFindSessionsComplete(bool Success);
	void OnJoinSessionsComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	void CreateSession();
	
};
