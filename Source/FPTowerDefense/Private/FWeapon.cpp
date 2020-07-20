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
	bIsReloading = false;

	MuzzleSocketName = "MuzzleFlashSocket";
	ShellEjectSocketName = "ShellEjectionSocket";
}

// Called when the game starts or when spawned
void AFWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	TimeBetweenShots = 60 / FireRate;
	CurrentMagCount = MaxMagCount;

	if (MeshComp && ArmReloadMontage && MeshComp->AnimScriptInstance)
	{
		MeshComp->AnimScriptInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &AFWeapon::OnReloadNotify);
	}
}


bool AFWeapon::StartFire()
{
	// if we try to fire on the same frame again the delay will still be TimeBetweenShots
	// Otherwise no delay on the next shot
	float FirstDelay = FMath::Max(LastFireTime + TimeBetweenShots - GetWorld()->TimeSeconds, 0.0f);

	if (CurrentMagCount > 0 && !bIsReloading && FirstDelay == 0.0f)
	{
		//TODO move animation calls for weapon and player here
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
	else if (CurrentMagCount == 0 && !bIsReloading)
	{
		StartReload();	
	}
	
	// when we can't fire, return false so that Owner doesn't run anims
	return false;
}

void AFWeapon::EndFire()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Fire);

	AActor* MyOwner = GetOwner();
	if (MyOwner)
	{
		// get owning character and call EndFire to stop animations
		AFCharacter* OwningCharacter = Cast<AFCharacter>(MyOwner);
		if (OwningCharacter)
		{
			OwningCharacter->EndFire();
		}
	}
}

bool AFWeapon::StartReload()
{
	if (CurrentMagCount < MaxMagCount && !bIsReloading)
	{
		EndFire();

		// Call reload animation here
		// consider moving to a separate ReloadAnim function
		USkeletalMeshComponent* UseMesh = MeshComp;
		if (MeshComp && ArmReloadMontage && UseMesh->AnimScriptInstance)
		{
			UseMesh->AnimScriptInstance->Montage_Play(GunReloadMontage, 1.0f);
		}

		AActor* MyOwner = GetOwner();
		if (MyOwner)
		{
			AFCharacter* OwningCharacter = Cast<AFCharacter>(MyOwner);
			// call reload animation on character ArmMesh
			if (OwningCharacter)
			{
				USkeletalMeshComponent* CharArmMesh = OwningCharacter->GetArmMesh();
				if (CharArmMesh && ArmReloadMontage && CharArmMesh->AnimScriptInstance)
				{
					CharArmMesh->AnimScriptInstance->Montage_Play(ArmReloadMontage, 1.0f);
				}
			}
		}	

		bIsReloading = true;

		return true;
	}
	
	// when we can't reload, return false so that Owner doesn't run anims
	return false;
}

void AFWeapon::Fire()
{
	if (CurrentMagCount > 0)
	{	
		HandleFiring();
		LastFireTime = GetWorld()->TimeSeconds;
		// Have separate function to handle fire for different fire modes such as burst
	}
	else
	{
		EndFire();	
	}
}

void AFWeapon::HandleFiring()
{
	AActor* MyOwner = GetOwner();
	if (MyOwner)
	{
		// Play fire animation on owning Mesh
		AFCharacter* OwningCharacter = Cast<AFCharacter>(MyOwner);
		if (OwningCharacter)
		{
			USkeletalMeshComponent* UseMesh = OwningCharacter->GetArmMesh();
			if (UseMesh && FireMontage && UseMesh->AnimScriptInstance)
			{
				UseMesh->AnimScriptInstance->Montage_Play(FireMontage, 1.0f);
			}
		}
	}

	PlayWeaponFX();
	// Calls separate function to handle actual Fire trace
}

void AFWeapon::Reload()
{
	int ReloadCount = MaxMagCount - CurrentMagCount;
	CurrentMagCount += ReloadCount;
	bIsReloading = false;
}

void AFWeapon::OnReloadNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	if (NotifyName == "ReloadFinished")
	{
		Reload();
	}
}

void AFWeapon::PlayWeaponFX()
{
	if (MuzzleFlash)
	{
		FTransform MuzzleTransform = MeshComp->GetSocketTransform(MuzzleSocketName);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash, MuzzleTransform);
	}

	if (ShellEject)
	{
		FTransform EjectTransform = MeshComp->GetSocketTransform(ShellEjectSocketName);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ShellEject, EjectTransform);
	}
}

