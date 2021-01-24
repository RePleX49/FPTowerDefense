// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FAbility.generated.h"


class AAbilityTest;
class UObjectTest;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPTOWERDEFENSE_API UFAbility : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFAbility();

	void ActivateAbility();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UObjectTest* MyAbility;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	TSubclassOf<UObjectTest> SpawnClass;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	
};
