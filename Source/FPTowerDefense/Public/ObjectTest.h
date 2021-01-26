// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ObjectTest.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class FPTOWERDEFENSE_API UObjectTest : public UObject
{
	GENERATED_BODY()
	
public:
	virtual void UseAbility();

	UFUNCTION(BlueprintNativeEvent)
	void UseAbilityBP();

	void UpdateCooldown(float UpdateVal);

	bool IsAbilityReady();

	virtual UWorld* GetWorld() const override 
	{
		if (const UObject* MyOuter = GetOuter())
		{
			return MyOuter->GetWorld();
		}

		return nullptr;
	}

	UActorComponent* GetOwningActor() const 
	{
		return GetTypedOuter<UActorComponent>();
	}

	UFUNCTION(BlueprintGetter)
	float GetCooldownDuration();
	
	UFUNCTION(BlueprintGetter)
	float GetCurrentCooldown();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	TSubclassOf<AActor> SpawnClass;

	UPROPERTY(BlueprintGetter=GetCooldownDuration, EditDefaultsOnly, Category = "Properties")
	float CooldownDuration;

	UPROPERTY(BlueprintGetter=GetCurrentCooldown)
	float CurrentCooldown;
};
