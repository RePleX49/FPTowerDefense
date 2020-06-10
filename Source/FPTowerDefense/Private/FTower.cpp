// Fill out your copyright notice in the Description page of Project Settings.


#include "FTower.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFTower::AFTower()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

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
	
	GetWorldTimerManager().SetTimer(TimerHandle_FindTarget, this, &AFTower::FindTarget, 0.0167f, true, 0.0f);
}

void AFTower::Attack()
{
	if (CurrentTarget)
	{
		FVector TraceStart = GetActorLocation();
		FVector TraceEnd = CurrentTarget->GetTargetLocation();

		if (GetDistanceTo(CurrentTarget) > RangeRadius)
		{
			GetWorldTimerManager().ClearTimer(TimerHandle_Attack);
			GetWorldTimerManager().SetTimer(TimerHandle_FindTarget, this, &AFTower::FindTarget, 0.0167f, true, 0.0f);
		}

		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0f, 0, 2.0f);

		UGameplayStatics::ApplyDamage(CurrentTarget, 5.0f, nullptr, this, DamageType);
	}
}

void AFTower::FindTarget()
{
	TArray<AActor*> OverlappedActors;
	SphereComp->GetOverlappingActors(OverlappedActors);

	for (AActor* actor : OverlappedActors)
	{
		if (actor != this)
		{
			//UE_LOG(LogTemp, Warning, TEXT("%s in Range"), *actor->GetName());
			// TODO sort target by health, or distance

		}
	}

	// greater than one to ignore self
	if (OverlappedActors.Num() > 1)
	{
		// temporary get target
		CurrentTarget = OverlappedActors[1];
		GetWorldTimerManager().ClearTimer(TimerHandle_FindTarget);
		GetWorldTimerManager().SetTimer(TimerHandle_Attack, this, &AFTower::Attack, AttackSpeed, true, 0.0f);
	}
}

// Called every frame
void AFTower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

