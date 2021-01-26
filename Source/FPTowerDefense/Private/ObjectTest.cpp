// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectTest.h"

void UObjectTest::UseAbility()
{
	if (SpawnClass)
	{
		AActor* MyOwner = GetOwningActor()->GetOwner();
		if (MyOwner)
		{
			FActorSpawnParameters SpawnParams;
			UObjectTest::GetWorld()->SpawnActor<AActor>(SpawnClass, MyOwner->GetActorLocation() + FVector(0.0f, 100.0f, 0.0f), MyOwner->GetActorRotation(), SpawnParams);
		}
	}

	CurrentCooldown = 0.0f;
}

void UObjectTest::UpdateCooldown(float UpdateVal)
{
	CurrentCooldown += UpdateVal;
	UE_LOG(LogTemp, Warning, TEXT("Current Cooldown: %f"), CurrentCooldown);
}

bool UObjectTest::IsAbilityReady()
{
	if (CurrentCooldown >= CooldownDuration)
	{
		return true;
	}

	return false;
}

float UObjectTest::GetCooldownDuration()
{
	return CooldownDuration;
}

float UObjectTest::GetCurrentCooldown()
{
	return CurrentCooldown;
}

void UObjectTest::UseAbilityBP_Implementation()
{
	UseAbility();
}
