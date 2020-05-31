// Fill out your copyright notice in the Description page of Project Settings.


#include "FWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "FCharacter.h"

// Sets default values
AFWeapon::AFWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	FireRate = 450.0f;
	MaxMagCount = 30;
	bIsAutomatic = true;

	MuzzleSocketName = "MuzzleFlashSocket";
}

// Called when the game starts or when spawned
void AFWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	TimeBetweenShots = 60 / FireRate;
	CurrentMagCount = MaxMagCount;
}


bool AFWeapon::StartFire()
{
	// if we try to fire on the same frame again the delay will still be TimeBetweenShots
	// Otherwise no delay on the next shot
	float FirstDelay = FMath::Max(LastFireTime + TimeBetweenShots - GetWorld()->TimeSeconds, 0.0f);

	if (CurrentMagCount > 0 && FirstDelay == 0.0f)
	{
		if (bIsAutomatic)
		{
			//TODO investigate removing redundancy with FirstDelay
			GetWorldTimerManager().SetTimer(TimerHandle_Fire, this, &AFWeapon::Fire, TimeBetweenShots, true, FirstDelay);
		}
		else
		{
			GetWorldTimerManager().SetTimer(TimerHandle_Fire, this, &AFWeapon::Fire, TimeBetweenShots, false, FirstDelay);
		}
		

		return true;
	}
	
	// when we can't fire, return false so that Owner doesn't run anims
	return false;
}

void AFWeapon::EndFire()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Fire);
}

bool AFWeapon::StartReload()
{
	if (CurrentMagCount < MaxMagCount)
	{
		// TODO play anim here and have actual reload on timer delay
		PlayReloadAnim();
		Reload();

		return true;
	}
	
	// when we can't reload, return false so that Owner doesn't run anims
	return false;
}

void AFWeapon::Fire()
{
	if (CurrentMagCount > 0)
	{
		LastFireTime = GetWorld()->TimeSeconds;
		CurrentMagCount--;
		PlayWeaponFX();
	}
	else
	{
		EndFire();

		// get owning character and call EndFire to stop animations
		AFCharacter* OwningCharacter = Cast<AFCharacter>(GetOwner());
		if (OwningCharacter)
		{
			OwningCharacter->EndFire();
		}
	}
}

void AFWeapon::Reload()
{
	int ReloadCount = MaxMagCount - CurrentMagCount;
	CurrentMagCount += ReloadCount;
}

void AFWeapon::PlayWeaponFX()
{
	FTransform MuzzleTransform = MeshComp->GetSocketTransform(MuzzleSocketName);

	if (MuzzleFlash)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash, MuzzleTransform);
	}
}

