// Fill out your copyright notice in the Description page of Project Settings.


#include "FHealthComponent.h"
#include "FEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UFHealthComponent::UFHealthComponent()
{
	MaxHealth = 100.0f;
	CurrentArmor = 0.0f;
}

// Called when the game starts
void UFHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* MyOwner = GetOwner();
	if (MyOwner)
	{
		MyOwner->OnTakeAnyDamage.AddDynamic(this, &UFHealthComponent::HandleTakeAnyDamage);

		AFEnemy* MyCharacter = Cast<AFEnemy>(MyOwner);
		if (MyCharacter)
		{
			DefaultMoveSpeed = MyCharacter->GetCharacterMovement()->MaxWalkSpeed;
		}
	}
	
	CurrentHealth = MaxHealth;
}


void UFHealthComponent::ResetMoveSpeed()
{
	AActor* MyOwner = GetOwner();
	if (MyOwner)
	{
		AFEnemy* MyCharacter = Cast<AFEnemy>(MyOwner);
		if (MyCharacter)
		{
			MyCharacter->GetCharacterMovement()->MaxWalkSpeed = DefaultMoveSpeed;
		}
	}

	//GetWorld()->GetTimerManager().ClearTimer(TimerHandle_ResetSlow);
}

void UFHealthComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, 
	AActor* DamageCauser)
{
	if (Damage <= 0.0f)
	{
		return;
	}

	// TODO Refactor damagetype check and how we hold different DamageType classes
	if (DamageType->IsA(SlowDamage))
	{
		UE_LOG(LogTemp, Warning, TEXT("Took slow damage"));
		AActor* MyOwner = GetOwner();
		if (MyOwner)
		{
			AFEnemy* MyCharacter = Cast<AFEnemy>(MyOwner);
			if (MyCharacter)
			{
				MyCharacter->GetCharacterMovement()->MaxWalkSpeed = 150.0f;
				GetWorld()->GetTimerManager().SetTimer(TimerHandle_ResetSlow, this, &UFHealthComponent::ResetMoveSpeed, 1.25f, false, 1.25f);
			}
		}
	}

	// Update Health value
	CurrentHealth = FMath::Clamp(CurrentHealth + CurrentArmor - Damage, 0.0f, MaxHealth);

	UE_LOG(LogTemp, Log, TEXT("%s Health: %s"), *GetOwner()->GetName(), *FString::SanitizeFloat(CurrentHealth));

	OnHealthChanged.Broadcast(this, CurrentHealth, Damage, DamageType, InstigatedBy, DamageCauser);
}