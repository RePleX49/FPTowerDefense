// Fill out your copyright notice in the Description page of Project Settings.


#include "FEnemy.h"
#include "FHealthComponent.h"
#include "FPTowerDefense/FPTowerDefense.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

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
	
	HealthComp->OnHealthChanged.AddDynamic(this, &AFEnemy::OnHealthChanged);
}

// Called every frame
void AFEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFEnemy::OnHealthChanged(UFHealthComponent* OwningHealthComp, float Health, float HealthDelta,
	const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (OwningHealthComp->IsDead())
	{
		GetMovementComponent()->StopMovementImmediately();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		DetachFromControllerPendingDestroy();
		//TODO Add dissolve affect on death
		SetLifeSpan(3.0f);
	}
}

// Called to bind functionality to input
void AFEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

