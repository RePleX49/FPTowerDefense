// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FWeapon.h"
#include "FWeapon_HitScan.generated.h"

/**
 * 
 */
UCLASS()
class FPTOWERDEFENSE_API AFWeapon_HitScan : public AFWeapon
{
	GENERATED_BODY()
	
protected:

	virtual void FireShot() override;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float WeaponRange;

};
