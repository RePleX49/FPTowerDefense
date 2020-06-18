// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FCharacter.h"
#include "FTrapper.generated.h"

/**
 * 
 */
UCLASS()
class FPTOWERDEFENSE_API AFTrapper : public AFCharacter
{
	GENERATED_BODY()
	
public:

	AFTrapper();

protected:

	void GetPlaceSpot();

	void PlaceTrap();

	virtual void UseOffensive() override;

	FTimerHandle TimerHandle_PlaceTrap;

	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	float HorizontalReach;

	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	float VerticalReach;

	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TSubclassOf<AActor> TrapClass;

	FVector TrapPlacement;

	// used in tick
	AActor* PreviousActor;

protected:

	virtual void Tick(float DeltaTime) override;

	void RemoveOutline(AActor* OutlinedActor);
};
