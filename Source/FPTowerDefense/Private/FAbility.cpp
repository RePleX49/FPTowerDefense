// Fill out your copyright notice in the Description page of Project Settings.


#include "FAbility.h"
#include "AbilityTest.h"
#include "ObjectTest.h"

// Sets default values for this component's properties
UFAbility::UFAbility()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UFAbility::TryAbility()
{
	if (MyAbility && MyAbility->IsAbilityReady())
	{
		ActivateAbility();
	}
}

void UFAbility::ActivateAbility()
{
	if (MyAbility)
	{
		MyAbility->UseAbilityBP();
	}
}

// Called when the game starts
void UFAbility::BeginPlay()
{
	Super::BeginPlay();

	// ...
	FActorSpawnParameters SpawnParams;

	if (SpawnClass)
	{
		MyAbility = NewObject<UObjectTest>(this, SpawnClass);
	}	
}


// Called every frame
void UFAbility::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (MyAbility)
	{
		if (!MyAbility->IsAbilityReady())
		{
			MyAbility->UpdateCooldown(DeltaTime);
		}
	}
}

