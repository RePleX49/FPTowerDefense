// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FTower.generated.h"

class USphereComponent;

UCLASS()
class FPTOWERDEFENSE_API AFTower : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFTower();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Attack();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* SphereComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret")
	float RangeRadius;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
