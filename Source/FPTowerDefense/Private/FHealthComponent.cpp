// Fill out your copyright notice in the Description page of Project Settings.


#include "FHealthComponent.h"
#include "FEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UFHealthComponent::UFHealthComponent()
{
	MaxHealth = 100.0f;
	MaxShield = 100.0f;

	SetIsReplicated(true);
}

void UFHealthComponent::Heal(float HealAmount)
{
	CurrentHealth = FMath::Clamp(CurrentHealth + HealAmount, 0.0f, MaxHealth);
}

void UFHealthComponent::RepairShield(float RepairAmount)
{
	CurrentShield = FMath::Clamp(CurrentShield + RepairAmount, 0.0f, MaxShield);

	UE_LOG(LogTemp, Log, TEXT("%s Shield: %s"), *GetOwner()->GetName(), *FString::SanitizeFloat(CurrentShield));
}

// Called when the game starts
void UFHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* MyOwner = GetOwner();
	if (MyOwner)
	{
		// only hook up if we are the server
		if (GetOwnerRole() == ROLE_Authority)
		{
			MyOwner->OnTakeAnyDamage.AddDynamic(this, &UFHealthComponent::HandleTakeAnyDamage);
		}

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
	float LeftOverDamage = 0.0f;

	if (CurrentShield > 0.0f)
	{
		CurrentShield = FMath::Clamp(CurrentShield - Damage, 0.0f, MaxShield);

		LeftOverDamage = CurrentShield - Damage;
		if (LeftOverDamage < 0.0f)
		{
			// since left over damage is already negative just add instead of subtracted
			CurrentHealth = FMath::Clamp(CurrentHealth + LeftOverDamage, 0.0f, MaxHealth);
		}
	}
	else
	{
		CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.0f, MaxHealth);
	}

	if (CurrentHealth <= 0.0f)
	{
		bIsDead = true;
	}

	UE_LOG(LogTemp, Log, TEXT("%s Shield: %s"), *GetOwner()->GetName(), *FString::SanitizeFloat(CurrentShield));
	UE_LOG(LogTemp, Log, TEXT("%s Health: %s"), *GetOwner()->GetName(), *FString::SanitizeFloat(CurrentHealth));

	OnHealthChanged.Broadcast(this, CurrentHealth, Damage, DamageType, InstigatedBy, DamageCauser);
}

void UFHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UFHealthComponent, CurrentHealth);
	DOREPLIFETIME(UFHealthComponent, CurrentShield);
	DOREPLIFETIME(UFHealthComponent, bIsDead);
}