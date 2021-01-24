// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityTest.h"

AAbilityTest::AAbilityTest()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AAbilityTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAbilityTest::UseAbility()
{
	if (SpawnClass)
	{
		AActor* MyOwner = GetOwner();
		if (MyOwner)
		{
			FActorSpawnParameters SpawnParams;
			GetWorld()->SpawnActor<AActor>(SpawnClass, MyOwner->GetActorLocation() + FVector(0.0f, 0.0f, 20.0f), MyOwner->GetActorRotation(), SpawnParams);
		}
	}
}
