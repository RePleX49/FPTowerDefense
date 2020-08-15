// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FTowerBase.generated.h"

class AFTower;
class USphereComponent;

UCLASS()
class FPTOWERDEFENSE_API AFTowerBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFTowerBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	USceneComponent* SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* TurretSlot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* SphereOverlap;

	AFTower* CurrentTower;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void BuildTower(TSubclassOf<AFTower> TowerType);

	USceneComponent* GetTurretSlot() const { return TurretSlot; };
};
