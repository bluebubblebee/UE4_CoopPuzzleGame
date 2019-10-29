// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactives/BasicInteractive.h"
#include "Door.generated.h"

/**
 * 
 */
UCLASS()
class COOPPUZZLEGAME_API ADoor : public ABasicInteractive
{
	GENERATED_BODY()


public:
	ADoor();

protected:
	virtual void BeginPlay() override;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door")
	class USceneComponent* ObjectToToggle;


	UPROPERTY(ReplicatedUsing = OnRep_IsOpenChanged, EditDefaultsOnly, Category = "Door")
	bool bIsOpen = false;

	// Event when bIsOpenChanges
	UFUNCTION()
	void OnRep_IsOpenChanged();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerDoActivatedAction();

	void DoActivatedAction();

	// Event only implemented on a blueprint
	UFUNCTION(BlueprintImplementableEvent)
	void OnDoorOpenedEvent();

public:

	virtual void StartInteracting(APawn* PawnInstigator) override;

	virtual void SendSignalToInteractive() override;	
};
