// Fill out your copyright notice in the Description page of Project Settings.


#include "FPTowerDefense/Public/Towers/FTowerBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "FTower.h"
#include "FPTowerDefense/FPTowerDefense.h"

// Sets default values
AFTowerBase::AFTowerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceneRoot;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(RootComponent);

	SphereOverlap = CreateDefaultSubobject<USphereComponent>(TEXT("InteractOverlap"));
	SphereOverlap->SetupAttachment(RootComponent);
	SphereOverlap->SetCollisionResponseToChannel(COLLISION_PLAYER, ECR_Overlap);
	SphereOverlap->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Overlap);

	TurretSlot = CreateDefaultSubobject<USceneComponent>(TEXT("TurretSlot"));
	TurretSlot->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AFTowerBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFTowerBase::BuildTower(TSubclassOf<AFTower> TowerType)
{
	if (!CurrentTower)
	{
		FActorSpawnParameters ActorSpawnParam;
		ActorSpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		CurrentTower = GetWorld()->SpawnActor<AFTower>(TowerType, TurretSlot->GetComponentTransform(), ActorSpawnParam);
	}
}

// Called every frame
void AFTowerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

