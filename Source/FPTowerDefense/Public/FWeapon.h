// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FWeapon.generated.h"

UCLASS()
class FPTOWERDEFENSE_API AFWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFWeapon();

	bool StartFire();
	void EndFire();
	virtual bool StartReload();

	bool GetIsAutomatic() { return bIsAutomatic; };

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* MeshComp;

	virtual void Fire();

	// function that actually calls Traces
	virtual void HandleFiring();

	virtual void FireShot() PURE_VIRTUAL(AFWeapon::FireShot,);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void Reload();

	// function call for playing animations in blueprint
	UFUNCTION()
	void OnReloadNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);

	bool bIsReloading;

	virtual void PlayWeaponFX();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	bool bIsAutomatic;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	float BaseDamage;

	float TimeBetweenShots;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	float FireRate;

	float LastFireTime;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Weapon")
	int CurrentMagCount;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (ClampMin = 0.0f))
	int MaxMagCount;

	UPROPERTY(EditDefaultsOnly, Category = "FX")
	UParticleSystem* MuzzleFlash;

	UPROPERTY(EditDefaultsOnly, Category = "FX")
	UParticleSystem* ShellEject;

	UPROPERTY(EditDefaultsOnly, Category = "FX")
	UParticleSystem* DefaultImpactEffect;

	UPROPERTY(EditDefaultsOnly, Category = "FX")
	UParticleSystem* FleshImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anim")
	UAnimMontage* ArmReloadMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anim")
	UAnimMontage* GunReloadMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anim")
	UAnimMontage* FireMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anim")
	UAnimBlueprint* ArmsAnimBP;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<UDamageType> DamageType;

	FTimerHandle TimerHandle_Fire;

	UPROPERTY(EditDefaultsOnly, Category = "FX")
	FName MuzzleSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "FX")
	FName ShellEjectSocketName;

public:	

};
