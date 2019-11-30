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

	void Setup(ISessionMenuInterface* SessionMenuInterface);

	void Teardown();


protected:
	virtual bool Initialize() override;

protected:
	ISessionMenuInterface* SessionMenuInterface;


protected:

	// Menus
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* MenuSwitcher;

	
	UPROPERTY(meta = (BindWidget))
	class UWidget* HostSessionMenuWidget;

	


	// Link to buttons
	UPROPERTY(meta = (BindWidget))
	class UButton* NewSessionButton;

	UFUNCTION()
	void OnNewSessionPressed();

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinSessionButton;

	UFUNCTION()
	void OnJoinSessionPressed();


	//// JOIN SESSIONS ///////
protected:

	UPROPERTY(meta = (BindWidget))
	class UWidget* SessionListMenuWidget;

	UPROPERTY(meta = (BindWidget))
	class UPanelWidget* ScrollSessionList;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class USessionRow> SessionRowClass;
	TOptional<uint32> SelectedScrollIndex;

	// Buttons

	UPROPERTY(meta = (BindWidget))
	class UButton* CancelJoinSessionButton;
	
	UFUNCTION()
	void OnCancelJoinSession();

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinSelectedSessionButton;
	
	UFUNCTION()
	void OnJoinSelectedSession();


	//// JOIN SESSIONS ///////


	
};
