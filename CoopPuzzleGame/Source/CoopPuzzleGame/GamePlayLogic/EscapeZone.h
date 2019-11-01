// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EscapeZone.generated.h"

UCLASS()
class COOPPUZZLEGAME_API AEscapeZone : public AActor
{
	GENERATED_BODY()

public:

	AEscapeZone();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EscapeZone")
	class USceneComponent* RootScene;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EscapeZone")
	class UBoxComponent* Collision;


	UFUNCTION()
	void HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


protected:
	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

private:	

	UPROPERTY(Replicated, EditDefaultsOnly, Category = "EscapeZone")
	int32 PlayerCount;
};
