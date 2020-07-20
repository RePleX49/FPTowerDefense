// Fill out your copyright notice in the Description page of Project Settings.


#include "FCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "FWeapon.h"
#include "FHealthComponent.h"

// Sets default values
AFCharacter::AFCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->bUsePawnControlRotation = true;
	CameraComp->SetupAttachment(RootComponent);
	
	ArmMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Arm Mesh"));
	ArmMesh->SetupAttachment(CameraComp);

	HealthComp = CreateDefaultSubobject<UFHealthComponent>(TEXT("HealthComp"));

	SprintSpeed = 550.0f;
	bIsFiring = false;

	WeaponAttachSocketName = "AttachSocket";
}

// Called when the game starts or when spawned
void AFCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	DefaultWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	EquippedWeapon = GetWorld()->SpawnActor<AFWeapon>(PrimaryWeapon, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	if (EquippedWeapon)
	{
		EquippedWeapon->SetOwner(this);
		EquippedWeapon->AttachToComponent(ArmMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponAttachSocketName);
	}
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
	if (!bIsFiring)
	{
		if (EquippedWeapon)
		{
			if (EquippedWeapon->StartFire())
			{
				bIsFiring = true;
				StopSprint();
			}
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

void AFCharacter::UseOffensive()
{
	UE_LOG(LogTemp, Warning, TEXT("Used Offensive Ability"));
	UseOffensiveBP();
}

void AFCharacter::UseSupport()
{
	UE_LOG(LogTemp, Warning, TEXT("Used Support Ability"));
	UseSupportBP();
}

// Called every frame
void AFCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

	PlayerInputComponent->BindAction("Offensive", IE_Pressed, this, &AFCharacter::UseOffensive);
	PlayerInputComponent->BindAction("Support", IE_Pressed, this, &AFCharacter::UseSupport);
}

