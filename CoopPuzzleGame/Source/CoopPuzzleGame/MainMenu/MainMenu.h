// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SessionMenuInterface.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class COOPPUZZLEGAME_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	//UMainMenu(const FObjectInitializer & ObjectInitializer);


	void Setup(ISessionMenuInterface* SessionMenuInterface);

	void Teardown();


protected:
	virtual bool Initialize() override;

protected:
	ISessionMenuInterface* SessionMenuInterface;


protected:

	// Link to buttons
	UPROPERTY(meta = (BindWidget))
	class UButton* NewSessionButton;

	UFUNCTION()
	void OnNewSessionPressed();

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinSessionButton;

	UFUNCTION()
	void OnJoinSessionPressed();

	
};
