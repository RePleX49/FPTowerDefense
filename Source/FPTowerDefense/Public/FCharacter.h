// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DrawDebugHelpers.h"
#include "FCharacter.generated.h"

class UCameraComponent;
class AFWeapon;
class UFHealthComponent;
class AFTowerBase;
class AFTower;

UCLASS()
class FPTOWERDEFENSE_API AFCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFCharacter();

	void EndFire();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);

	void MoveRight(float Value);

	void Sprint();

	void StopSprint();
	
	virtual void StartFire();

	void Reload();

	virtual void UseAbilityA();

	virtual void UseAbilityB();

	UFUNCTION(BlueprintImplementableEvent, Category = "Ability")
	void UseSupportBP();

	UFUNCTION(BlueprintImplementableEvent, Category = "Ability")
	void UseOffensiveBP();

	void GetTurretPlacement();

	void EnterBuildMode();

	void BuildTurret();

	UFUNCTION()
	void OnHealthChanged(UFHealthComponent* OwningHealthComp, float Health, float HealthDelta, 
		const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* ArmMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UFHealthComponent* HealthComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<AFWeapon> PrimaryWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "Turrets")
	TSubclassOf<AFTower> TurretClass;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Weapon")
	AFWeapon* EquippedWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName WeaponAttachSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Properties")
	float SprintSpeed;

	float DefaultWalkSpeed;

	bool bIsFiring;

	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	float CooldownRate;

	UPROPERTY(BlueprintReadOnly, Category = "Ability")
	float Cooldown_Support;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	float CooldownTime_Support;

	UPROPERTY(BlueprintReadOnly, Category = "Ability")
	float Cooldown_Offensive;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	float CooldownTime_Offensive;

	bool bInBuildMode;

	FVector TurretPlacement;

	FTimerHandle TimerHandle_Turret;

	AFTowerBase* SelectedBase;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual FVector GetPawnViewLocation() const override;

	USkeletalMeshComponent* GetArmMesh() const { return ArmMesh; };

	UCameraComponent* GetCameraComp() const { return CameraComp; };

};
