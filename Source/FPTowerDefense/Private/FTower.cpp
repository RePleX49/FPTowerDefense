// Fill out your copyright notice in the Description page of Project Settings.


#include "FTower.h"
#include "Components/SphereComponent.h"

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
}

// Called when the game starts or when spawned
void AFTower::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFTower::Attack()
{

}

// Called every frame
void AFTower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<AActor*> OverlappedActors;
	SphereComp->GetOverlappingActors(OverlappedActors);

	for (AActor* actor : OverlappedActors)
	{
		if (actor != this)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s in Range"), *actor->GetName());
		}	
	}

}

