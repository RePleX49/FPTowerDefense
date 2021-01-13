// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FHealthComponent.generated.h"

// OnHealthChanged event
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnHealthChangedSignature, UFHealthComponent*, OwningHealthComp, float, Health, float, DamageTaken, const class UDamageType*, DamageType, class AController*, InstigatedBy, AActor*, DamageCauser);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPTOWERDEFENSE_API UFHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFHealthComponent();

	bool IsDead() { return bIsDead; };

	UFUNCTION(BlueprintCallable, Category = "HealthComponent")
	void Heal(float HealAmount);

	UFUNCTION(BlueprintCallable, Category = "HealthComponent")
	void RepairShield(float RepairAmount);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "HealthComponent")
	TSubclassOf<UDamageType> SlowDamage;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "HealthComponent")
	float CurrentHealth;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category = "HealthComponent")
	float CurrentShield;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HealthComponent")
	float MaxHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HealthComponent")
	float MaxShield;

	float DefaultMoveSpeed;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "HealthComponent")
	bool bIsDead;

	FTimerHandle TimerHandle_ResetSlow;

	void ResetMoveSpeed();

	UFUNCTION()
	void HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy,
		AActor* DamageCauser);

public:

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnHealthChangedSignature OnHealthChanged;
		
};
