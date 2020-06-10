// Fill out your copyright notice in the Description page of Project Settings.


#include "FTrapper.h"
#include "Camera/CameraComponent.h"

void AFTrapper::GetPlaceSpot()
{
	FHitResult HitA, HitB;
	FVector TraceStart = CameraComp->GetComponentLocation();
	FVector TraceEnd = TraceStart + (CameraComp->GetForwardVector() * HorizontalReach);
	FCollisionQueryParams QueryParams;

	if (GetWorld()->LineTraceSingleByChannel(HitA, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
	{
		// draw decal to show placement spot
		DrawDebugLine(GetWorld(), TraceStart, HitA.Location, FColor::Red, false, 0.1f, 0, 0.25f);
		DrawDebugSphere(GetWorld(), HitA.Location, 25.0f, 12, FColor::Red, false, 0.1f, 0, 0.35f);
		TrapPlacement = HitA.Location;
	}
	else if (GetWorld()->LineTraceSingleByChannel(HitB, TraceEnd, TraceEnd + (FVector::UpVector * -VerticalReach), ECC_Visibility, QueryParams))
	{
		// if first trace didn't hit, trace down to check for floor
		DrawDebugLine(GetWorld(), TraceStart, HitA.Location, FColor::Red, false, 0.1f, 0, 0.25f);
		DrawDebugLine(GetWorld(), TraceEnd, HitB.Location, FColor::Red, false, 0.1f, 0, 0.25f);
		DrawDebugSphere(GetWorld(), HitB.Location, 25.0f, 12, FColor::Red, false, 0.1f, 0, 0.35f);
		TrapPlacement = HitB.Location;
	}
}

void AFTrapper::PlaceTrap()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_PlaceTrap);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	GetWorld()->SpawnActor<AActor>(TrapClass, TrapPlacement, GetActorRotation(), SpawnParams);

	InputComponent->BindAction("Fire", IE_Pressed, this, &AFTrapper::StartFire);
}

void AFTrapper::UseOffensive()
{
	if (!GetWorldTimerManager().IsTimerActive(TimerHandle_PlaceTrap))
	{
		//TODO fix rebinding to PlaceTrap function when using ability
		InputComponent->BindAction("Fire", IE_Pressed, this, &AFTrapper::PlaceTrap);
		GetWorldTimerManager().SetTimer(TimerHandle_PlaceTrap, this, &AFTrapper::GetPlaceSpot, 0.017f, true, 0.0f);
	}
	else
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_PlaceTrap);
	}
}
