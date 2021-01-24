// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "AbilityTest.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class FPTOWERDEFENSE_API AAbilityTest : public AInfo
{
	GENERATED_BODY()

public:
	AAbilityTest();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	TSubclassOf<AActor> SpawnClass;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void UseAbility();
};
