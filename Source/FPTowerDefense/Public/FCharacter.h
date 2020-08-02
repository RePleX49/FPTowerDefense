// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DrawDebugHelpers.h"
#include "FCharacter.generated.h"

class UCameraComponent;
class AFWeapon;
class UFHealthComponent;

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

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerAbilityA();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerAbilityB();

	virtual void UseAbilityA();

	virtual void UseAbilityB();

	UFUNCTION(BlueprintImplementableEvent, Category = "Ability")
	void UseSupportBP();

	UFUNCTION(BlueprintImplementableEvent, Category = "Ability")
	void UseOffensiveBP();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* ArmMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UFHealthComponent* HealthComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<AFWeapon> PrimaryWeapon;

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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	USkeletalMeshComponent* GetArmMesh() const { return ArmMesh; };

	UCameraComponent* GetCameraComp() const { return CameraComp; };

	// DEPRECATED function calls for playing animations through blueprint
	// Animation calls moved to FWeapon class
	/*
	UFUNCTION(BlueprintImplementableEvent, Category = "Anim")
	void PlayFireAnim();

	UFUNCTION(BlueprintImplementableEvent, Category = "Anim")
	void EndFireAnim();

	UFUNCTION(BlueprintImplementableEvent, Category = "Anim")
	void PlayReloadAnim();
	*/
	

};
