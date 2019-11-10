// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SessionMenuInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USessionMenuInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COOPPUZZLEGAME_API ISessionMenuInterface
{
	GENERATED_BODY()

public:


	virtual void Host(FString ServerName) = 0;

	virtual void JoinSession(uint32 Index) = 0;

	virtual void EndSession() = 0;

	//virtual void LoadMainMenu() = 0;

	//virtual void OpenSessionListMenu() = 0;

	
};
