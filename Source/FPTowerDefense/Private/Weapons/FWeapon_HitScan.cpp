// Fill out your copyright notice in the Description page of Project Settings.


#include "FPTowerDefense/Public/Weapons/FWeapon_HitScan.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "FPTowerDefense/FPTowerDefense.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Net/UnrealNetwork.h"

static int32 DebugWeaponDrawing = 0;
FAutoConsoleVariableRef CVARDebugWeaponDrawing(
	TEXT("FPTowerDefense.DebugWeapons"),
	DebugWeaponDrawing,
	TEXT("Draw Debug Lines for Weapons"), 
	ECVF_Cheat);

AFWeapon_HitScan::AFWeapon_HitScan()
{
	DamageMultiplier = 3.0f;
}

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

			HitScanTrace.SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());
			HitScanTrace.HitLocation = Hit.ImpactPoint;
			float WeakspotMult = 1.0f;

			if (HitScanTrace.SurfaceType == SURFACE_FLESHWEAK)
			{
				WeakspotMult = DamageMultiplier;
			}

			UGameplayStatics::ApplyPointDamage(HitActor, BaseDamage * WeakspotMult, ShotDirection, Hit, MyOwner->GetInstigatorController(),
				MyOwner, DamageType);			
		}

		CurrentMagCount--;
	}
}

void AFWeapon_HitScan::OnRep_ImpactParticle()
{
	PlayImpactFX();
}

void AFWeapon_HitScan::PlayImpactFX()
{
	UParticleSystem* SelectedEffect = nullptr;
	switch (HitScanTrace.SurfaceType)
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
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SelectedEffect, HitScanTrace.HitLocation, HitScanTrace.HitLocation.Rotation());
	}
}

void AFWeapon_HitScan::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFWeapon_HitScan, HitScanTrace);
}
