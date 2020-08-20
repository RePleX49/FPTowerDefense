// Fill out your copyright notice in the Description page of Project Settings.


#include "FCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "FWeapon.h"
#include "FHealthComponent.h"
#include "FPTowerDefense/FPTowerDefense.h"
#include "Net/UnrealNetwork.h"
#include "FPTowerDefense/Public/Towers/FTowerBase.h"

// Sets default values
AFCharacter::AFCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->bUsePawnControlRotation = true;
	CameraComp->SetupAttachment(RootComponent);
	
	ArmMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Arm Mesh"));
	ArmMesh->SetupAttachment(CameraComp);

	HealthComp = CreateDefaultSubobject<UFHealthComponent>(TEXT("HealthComp"));

	// assign capsule collision type to player and weapon collision response to ignore
	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionObjectType(COLLISION_PLAYER);

	SprintSpeed = 550.0f;
	bIsFiring = false;
	bInBuildMode = false;

	CooldownTime_Offensive = 4.0f;
	CooldownTime_Support = 4.0f;

	WeaponAttachSocketName = "AttachSocket";
}

// Called when the game starts or when spawned
void AFCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	DefaultWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;

	if (GetLocalRole() == ROLE_Authority)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		EquippedWeapon = GetWorld()->SpawnActor<AFWeapon>(PrimaryWeapon, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		if (EquippedWeapon)
		{
			EquippedWeapon->SetOwner(this);
			EquippedWeapon->AttachToComponent(ArmMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponAttachSocketName);
		}
	}

	HealthComp->OnHealthChanged.AddDynamic(this, &AFCharacter::OnHealthChanged);

	Cooldown_Support = CooldownTime_Support;
	Cooldown_Offensive = CooldownTime_Offensive;
}

void AFCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector() * Value);

	// cancel sprint when there is no forward input
	if (Value == 0)
	{
		if (!(GetCharacterMovement()->MaxWalkSpeed == DefaultWalkSpeed))
		{
			StopSprint();
		}
	}
}

void AFCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector() * Value);
}

void AFCharacter::Sprint()
{
	// toggle for sprint
	if (GetCharacterMovement()->MaxWalkSpeed == DefaultWalkSpeed)
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
	}
}

void AFCharacter::StopSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
}

void AFCharacter::StartFire()
{
	if (!bIsFiring && !bInBuildMode)
	{
		if (EquippedWeapon && EquippedWeapon->StartFire())
		{
			bIsFiring = true;
			StopSprint();
		}
	}
}

void AFCharacter::EndFire()
{
	if (bIsFiring)
	{
		bIsFiring = false;

		if (EquippedWeapon)
		{
			EquippedWeapon->EndFire();
		}
	}
}

void AFCharacter::Reload()
{
	if (EquippedWeapon)
	{
		if (EquippedWeapon->StartReload())
		{
			EndFire();
		}	
	}
}

void AFCharacter::UseAbilityA()
{
	if (Cooldown_Offensive == CooldownTime_Offensive)
	{
		UE_LOG(LogTemp, Warning, TEXT("Used Offensive Ability"));
		Cooldown_Offensive = 0.0f;
		UseOffensiveBP();
	}
}

void AFCharacter::UseAbilityB()
{
	if (Cooldown_Support == CooldownTime_Support)
	{
		UE_LOG(LogTemp, Warning, TEXT("Used Support Ability"));
		Cooldown_Support = 0.0f;
		UseSupportBP();
	}
}

void AFCharacter::GetTurretPlacement()
{
	FHitResult HitA;
	FVector TraceStart = CameraComp->GetComponentLocation();
	FVector TraceEnd = TraceStart + (CameraComp->GetForwardVector() * 3000.0f);
	FCollisionQueryParams QueryParams;

	if (GetWorld()->LineTraceSingleByChannel(HitA, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
	{
		if (HitA.GetActor() && HitA.GetActor()->IsA(AFTowerBase::StaticClass()))
		{
			SelectedBase = Cast<AFTowerBase>(HitA.GetActor());
			if (SelectedBase)
			{
				TurretPlacement = SelectedBase->GetTurretSlot()->GetComponentLocation(); // Change GetLocation to custom on Base
			}
			//TODO add error log
		}
		else
		{
			SelectedBase = nullptr;
			TurretPlacement = HitA.Location;
		}

		DrawDebugSphere(GetWorld(), TurretPlacement, 50.0f, 12, FColor::Green, false, 0.1f, 0, 0.35f);
	}
}

void AFCharacter::EnterBuildMode()
{
	if (!GetWorldTimerManager().IsTimerActive(TimerHandle_Turret))
	{
		bInBuildMode = true;
		GetWorldTimerManager().SetTimer(TimerHandle_Turret, this, &AFCharacter::GetTurretPlacement, 0.0167f, true, 0.0f);	
	}
	else
	{
		bInBuildMode = false;
		GetWorldTimerManager().ClearTimer(TimerHandle_Turret);
	}
}

void AFCharacter::BuildTurret()
{
	if (bInBuildMode && SelectedBase)
	{
		SelectedBase->BuildTower(TurretClass);
		bInBuildMode = false;
		GetWorldTimerManager().ClearTimer(TimerHandle_Turret);
	}
}

void AFCharacter::OnHealthChanged(UFHealthComponent* OwningHealthComp, float Health, float HealthDelta,
	const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (OwningHealthComp->IsDead())
	{
		GetMovementComponent()->StopMovementImmediately();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		DetachFromControllerPendingDestroy();
	}
}

// Called every frame
void AFCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Cooldown_Support < CooldownTime_Support)
	{
		float CooldownVal = Cooldown_Support + (CooldownRate * DeltaTime);
		Cooldown_Support = FMath::Clamp(CooldownVal, 0.0f, CooldownTime_Support);
	}

	if (Cooldown_Offensive < CooldownTime_Offensive)
	{
		float CooldownVal = Cooldown_Offensive + (CooldownRate * DeltaTime);
		Cooldown_Offensive = FMath::Clamp(CooldownVal, 0.0f, CooldownTime_Offensive);
	}
}

// Called to bind functionality to input
void AFCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Movement and look control setup
	PlayerInputComponent->BindAxis("MoveForward", this, &AFCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFCharacter::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &AFCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &AFCharacter::AddControllerYawInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFCharacter::Jump);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AFCharacter::Sprint);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFCharacter::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AFCharacter::EndFire);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AFCharacter::Reload);

	PlayerInputComponent->BindAction("Offensive", IE_Pressed, this, &AFCharacter::UseAbilityA);
	PlayerInputComponent->BindAction("Support", IE_Pressed, this, &AFCharacter::UseAbilityB);
	PlayerInputComponent->BindAction("Build", IE_Pressed, this, &AFCharacter::EnterBuildMode);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFCharacter::BuildTurret);
}

FVector AFCharacter::GetPawnViewLocation() const
{
	if (CameraComp)
	{
		return CameraComp->GetComponentLocation();
	}

	return GetActorLocation() + FVector(0.f, 0.f, BaseEyeHeight);
}

void AFCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFCharacter, EquippedWeapon);
}