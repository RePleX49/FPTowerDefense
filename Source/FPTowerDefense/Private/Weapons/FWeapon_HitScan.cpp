// Fill out your copyright notice in the Description page of Project Settings.


#include "FPTowerDefense/Public/Weapons/FWeapon_HitScan.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "FPTowerDefense/FPTowerDefense.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

static int32 DebugWeaponDrawing = 0;
FAutoConsoleVariableRef CVARDebugWeaponDrawing(
	TEXT("FPTowerDefense.DebugWeapons"),
	DebugWeaponDrawing,
	TEXT("Draw Debug Lines for Weapons"), 
	ECVF_Cheat);

void AFWeapon_HitScan::FireShot()
{
	AActor* MyOwner = GetOwner();
	if (MyOwner)
	{
		FVector ViewLocation;
		FRotator ViewRotation;

		MyOwner->GetActorEyesViewPoint(ViewLocation, ViewRotation);

		FVector ShotDirection = ViewRotation.Vector();
		FVector TraceEnd = ViewLocation + (ShotDirection * WeaponRange);

		FHitResult Hit;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(MyOwner);
		QueryParams.AddIgnoredActor(this);
		QueryParams.bTraceComplex = true;
		QueryParams.bReturnPhysicalMaterial = true;

		if (DebugWeaponDrawing > 0)
		{
			DrawDebugLine(GetWorld(), ViewLocation, TraceEnd, FColor::Red, true, 2.0f, 0, 2.5f);
		}

		if (GetWorld()->LineTraceSingleByChannel(Hit, ViewLocation, TraceEnd, COLLISION_WEAPON, QueryParams))
		{
			AActor* HitActor = Hit.GetActor();

			UGameplayStatics::ApplyPointDamage(HitActor, BaseDamage, ShotDirection, Hit, MyOwner->GetInstigatorController(),
				MyOwner, DamageType);

			EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());

			UParticleSystem* SelectedEffect = nullptr;
			switch (SurfaceType)
			{
			case SURFACE_FLESHDEFAULT:
			case SURFACE_FLESHWEAK:
				SelectedEffect = FleshImpactEffect;
				break;

			default:
				SelectedEffect = DefaultImpactEffect;
				break;
			}

			if (SelectedEffect)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SelectedEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
			}
		}

		CurrentMagCount--;

		//TODO add burst cancel to reload function
	}
}

