// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPTowerDefense/Public/Weapons/FWeapon_HitScan.h"
#include "FBurstRifle.generated.h"

/**
 * 
 */
UCLASS()
class FPTOWERDEFENSE_API AFBurstRifle : public AFWeapon_HitScan
{
	GENERATED_BODY()

public:
	virtual bool StartReload() override;

protected:

	virtual void HandleFiring() override;

	virtual void FireShot() override;

	void StopBurst();

	int CurrentBurst;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	int BurstCount;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float TimeBetweenBurst;

	float LastBurstTime;

	FTimerHandle TimerHandle_Burst;
};
