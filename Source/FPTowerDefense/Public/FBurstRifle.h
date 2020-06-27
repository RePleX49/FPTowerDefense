// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FWeapon.h"
#include "FBurstRifle.generated.h"

/**
 * 
 */
UCLASS()
class FPTOWERDEFENSE_API AFBurstRifle : public AFWeapon
{
	GENERATED_BODY()
	
protected:

	virtual void HandleFiring() override;

	virtual void FireShot() override;

	void BurstFire();

	int CurrentBurst;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	int BurstCount;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float WeaponRange;
};
