// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FWeapon.h"
#include "FWeapon_HitScan.generated.h"

USTRUCT()
struct FHitScanTrace
{
	GENERATED_BODY()

public:
	UPROPERTY()
		TEnumAsByte<EPhysicalSurface> SurfaceType;

	UPROPERTY()
		FVector_NetQuantize HitLocation;
};

/**
 * 
 */
UCLASS()
class FPTOWERDEFENSE_API AFWeapon_HitScan : public AFWeapon
{
	GENERATED_BODY()

public:
	AFWeapon_HitScan();
	
protected:

	virtual void FireShot() override;

	void PlayImpactFX();

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float WeaponRange;

	UPROPERTY(ReplicatedUsing = OnRep_ImpactParticle)
	FHitScanTrace HitScanTrace;

	UFUNCTION()
	void OnRep_ImpactParticle();

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float DamageMultiplier;
};
