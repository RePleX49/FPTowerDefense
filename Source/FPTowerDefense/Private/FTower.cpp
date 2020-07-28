// Fill out your copyright notice in the Description page of Project Settings.


#include "FTower.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "FEnemy.h"
#include "FHealthComponent.h"

// Sets default values
AFTower::AFTower()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh Comp"));
	RootComponent = MeshComp;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Comp"));
	SphereComp->SetSphereRadius(RangeRadius);
	SphereComp->SetupAttachment(MeshComp);

	AttackSpeed = 0.2f;
	
}

// Called when the game starts or when spawned
void AFTower::BeginPlay()
{
	Super::BeginPlay();
	
	SphereComp->SetSphereRadius(RangeRadius);
	CurrentTarget = nullptr;
}

void AFTower::Attack()
{
	if (CurrentTarget)
	{
		HandleAttack();
	}
	else
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_Attack);
	}
}

void AFTower::HandleAttack()
{
	FVector TraceStart = GetActorLocation();
	FVector TraceEnd = CurrentTarget->GetTargetLocation();

	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0f, 0, 2.0f);

	UGameplayStatics::ApplyDamage(CurrentTarget, 5.0f, nullptr, this, DamageType);

	// if target returns dead after hit clear target reference
	UFHealthComponent* TargetHealthComp = nullptr;
	TargetHealthComp = Cast<UFHealthComponent>(CurrentTarget->GetComponentByClass(UFHealthComponent::StaticClass()));

	if (TargetHealthComp && TargetHealthComp->IsDead())
	{
		CurrentTarget = nullptr;
		UE_LOG(LogTemp, Warning, TEXT("Cleared target"));
	}
}

void AFTower::FindTarget()
{
	TArray<AActor*> OverlappedActors;
	SphereComp->GetOverlappingActors(OverlappedActors, AFEnemy::StaticClass());

	for (AActor* actor : OverlappedActors)
	{
		//UE_LOG(LogTemp, Warning, TEXT("%s in Range"), *actor->GetName());
		// TODO sort target by health, or distance
		
		UFHealthComponent* TargetHealthComp;
		TargetHealthComp = Cast<UFHealthComponent>(actor->GetComponentByClass(UFHealthComponent::StaticClass()));

		// check if potential target is alive
		if (TargetHealthComp && !TargetHealthComp->IsDead())
		{
			if (CurrentTarget == nullptr)
			{
				CurrentTarget = actor;
				break;
			}
		}
	}

	if (CurrentTarget)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_Attack, this, &AFTower::Attack, AttackSpeed, true, 0.0f);
	}
}

// Called every frame
void AFTower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentTarget == nullptr)
	{
		FindTarget();
	}

	if (GetDistanceTo(CurrentTarget) > RangeRadius)
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_Attack);
		CurrentTarget = nullptr;
	}
}

