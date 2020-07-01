// Fill out your copyright notice in the Description page of Project Settings.


#include "FBurstRifle.h"

void AFBurstRifle::HandleFiring()
{
	Super::HandleFiring();

	// Check which Burst shot we are on 
	if (CurrentBurst < BurstCount && CurrentMagCount > 0)
	{
		float FirstBurstDelay = FMath::Max(TimeBetweenBurst + LastBurstTime - GetWorld()->TimeSeconds, 0.0f);
		GetWorldTimerManager().SetTimer(TimerHandle_Burst, this, &AFBurstRifle::FireShot, TimeBetweenBurst, false, FirstBurstDelay);
	}
	else
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_Burst);
		EndFire();
		CurrentBurst = 0;
	}
}

void AFBurstRifle::FireShot()
{
	Super::FireShot();

	LastBurstTime = GetWorld()->TimeSeconds;
	CurrentBurst++;
	HandleFiring();
}
