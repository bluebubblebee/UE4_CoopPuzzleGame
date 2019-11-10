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



	
};
