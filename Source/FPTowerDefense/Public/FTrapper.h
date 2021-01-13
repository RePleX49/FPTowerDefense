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

	virtual void StartFire();

	void GetPlaceSpot();

	void PlaceTrap();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerPlaceTrap();

	virtual void UseAbilityA() override;

	virtual void AbilityA() override;

	virtual void AbilityB() override;

	FTimerHandle TimerHandle_PlaceTrap;

	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	float HorizontalReach;

	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	float VerticalReach;

	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TSubclassOf<AActor> TrapClass;

	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TSubclassOf<AActor> TetherClass;

	UPROPERTY(Replicated)
	FVector TrapPlacement;

	// used in tick
	AActor* PreviousActor;

	bool bInPlacementMode;

protected:

	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void RemoveOutline(AActor* OutlinedActor);
};
