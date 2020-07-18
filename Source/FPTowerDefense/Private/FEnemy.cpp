// Fill out your copyright notice in the Description page of Project Settings.


#include "FEnemy.h"
#include "FHealthComponent.h"
#include "FPTowerDefense/FPTowerDefense.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AFEnemy::AFEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComp = CreateDefaultSubobject<UFHealthComponent>(TEXT("Health Comp"));
}

// Called when the game starts or when spawned
void AFEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

