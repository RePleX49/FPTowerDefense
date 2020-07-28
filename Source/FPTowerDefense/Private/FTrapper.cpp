// Fill out your copyright notice in the Description page of Project Settings.


#include "FTrapper.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"

AFTrapper::AFTrapper()
{
	PrimaryActorTick.bCanEverTick = true;

	bInPlacementMode = false;
}

void AFTrapper::StartFire()
{
	if (!bInPlacementMode)
	{
		Super::StartFire();
	}
}

void AFTrapper::GetPlaceSpot()
{
	FHitResult HitA, HitB;
	FVector TraceStart = CameraComp->GetComponentLocation();
	FVector TraceEnd = TraceStart + (CameraComp->GetForwardVector() * HorizontalReach);
	FCollisionQueryParams QueryParams;

	if (GetWorld()->LineTraceSingleByChannel(HitA, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
	{
		// draw decal to show placement spot

		/*
		DrawDebugLine(GetWorld(), TraceStart, HitA.Location, FColor::Red, false, 0.1f, 0, 0.25f);
		*/
		
		DrawDebugSphere(GetWorld(), HitA.Location, 25.0f, 12, FColor::Red, false, 0.1f, 0, 0.35f);

		TrapPlacement = HitA.Location;
	}
	else if (GetWorld()->LineTraceSingleByChannel(HitB, TraceEnd, TraceEnd + (FVector::UpVector * -VerticalReach), ECC_Visibility, QueryParams))
	{
		// if first trace didn't hit, trace down to check for floor

		/*
		DrawDebugLine(GetWorld(), TraceStart, HitA.Location, FColor::Red, false, 0.1f, 0, 0.25f);
		DrawDebugLine(GetWorld(), TraceEnd, HitB.Location, FColor::Red, false, 0.1f, 0, 0.25f);
		
		*/
		DrawDebugSphere(GetWorld(), HitB.Location, 25.0f, 12, FColor::Red, false, 0.1f, 0, 0.35f);

		TrapPlacement = HitB.Location;
	}
}

void AFTrapper::PlaceTrap()
{
	if (bInPlacementMode)
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_PlaceTrap);

		if (TrapClass)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			GetWorld()->SpawnActor<AActor>(TrapClass, TrapPlacement, GetActorRotation(), SpawnParams);

			UE_LOG(LogTemp, Warning, TEXT("Placed Trap"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Could not find trap class"));
		}

		bInPlacementMode = false;
	}
}

void AFTrapper::UseOffensive()
{
	if (!GetWorldTimerManager().IsTimerActive(TimerHandle_PlaceTrap))
	{
		bInPlacementMode = true;
		GetWorldTimerManager().SetTimer(TimerHandle_PlaceTrap, this, &AFTrapper::GetPlaceSpot, 0.017f, true, 0.0f);
	}
	else
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_PlaceTrap);
	}
}

void AFTrapper::UseSupport()
{
	// throw tether projectile
	if (TetherClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		// Get a direction vector from SpawnLoc to point where player is looking
		FVector SpawnLocation = CameraComp->GetComponentLocation() + (GetActorRightVector() * -65.0f);
		FVector LookAtPosition = CameraComp->GetComponentLocation() + (CameraComp->GetForwardVector() * 2000.0f);
		FVector ThrowDirection = LookAtPosition - SpawnLocation;

		GetWorld()->SpawnActor<AActor>(TetherClass, SpawnLocation, ThrowDirection.Rotation(), SpawnParams);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Could not find tether class"));
	}
	
}

void AFTrapper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector TraceStart = CameraComp->GetComponentLocation();
	FVector TraceEnd = TraceStart + (CameraComp->GetForwardVector() * 1000.0f);

	FHitResult Hit;
	FCollisionQueryParams QueryParams;

	if (GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
	{
		if (Hit.GetActor() && Hit.GetActor()->IsA(TrapClass))
		{
			// TODO refactor actor checks
			if (PreviousActor)
			{
				if (Hit.GetActor() == PreviousActor)
				{
					return;
				}
				else
				{
					RemoveOutline(PreviousActor);
				}		
			}

			UActorComponent* ActorComp = Hit.GetActor()->GetComponentByClass(UStaticMeshComponent::StaticClass());
			if (ActorComp)
			{
				UStaticMeshComponent* PrimComp = Cast<UStaticMeshComponent>(ActorComp);
				PrimComp->SetRenderCustomDepth(true);
			}

			PreviousActor = Hit.GetActor();
		}
		else if (PreviousActor)
		{
			RemoveOutline(PreviousActor);
			PreviousActor = nullptr;
		}
	}
	else
	{
		if (PreviousActor)
		{
			RemoveOutline(PreviousActor);
			PreviousActor = nullptr;
		}
	}
}

void AFTrapper::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAction("Fire", IE_Pressed, this, &AFTrapper::PlaceTrap);

}

void AFTrapper::RemoveOutline(AActor* OutlinedActor)
{
	// stop rendering outline
	UActorComponent* ActorComp = OutlinedActor->GetComponentByClass(UStaticMeshComponent::StaticClass());

	if (ActorComp)
	{
		UStaticMeshComponent* PrimComp = Cast<UStaticMeshComponent>(ActorComp);
		PrimComp->SetRenderCustomDepth(false);
	}
}
