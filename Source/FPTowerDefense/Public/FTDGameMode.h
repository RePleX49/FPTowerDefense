// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FTDGameMode.generated.h"

/**
 * 
 */
UCLASS()
class FPTOWERDEFENSE_API AFTDGameMode : public AGameModeBase
{
	GENERATED_UCLASS_BODY()

	virtual UClass* GetDefaultPawnClassForController(AController* InController) override;
};
