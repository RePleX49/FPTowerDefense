// Fill out your copyright notice in the Description page of Project Settings.


#include "FHealthComponent.h"

// Sets default values for this component's properties
UFHealthComponent::UFHealthComponent()
{
	MaxHealth = 100.0f;
}


// Called when the game starts
void UFHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* MyOwner = GetOwner();
	if (MyOwner)
	{
		MyOwner->OnTakeAnyDamage.AddDynamic(this, &UFHealthComponent::HandleTakeAnyDamage);
	}
	
	CurrentHealth = MaxHealth;
}


void UFHealthComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, 
	AActor* DamageCauser)
{
	if (Damage <= 0.0f)
	{
		return;
	}

	// Update Health value
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.0f, MaxHealth);

	UE_LOG(LogTemp, Log, TEXT("%s Health: %s"), *GetOwner()->GetName(), *FString::SanitizeFloat(CurrentHealth));

	OnHealthChanged.Broadcast(this, CurrentHealth, Damage, DamageType, InstigatedBy, DamageCauser);
}