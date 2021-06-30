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
	GENERATED_BODY()

public:
    AFTDGameMode();

    virtual void StartPlay() override;

    UFUNCTION(BlueprintCallable)
    void PlayerReadyUp();

    UFUNCTION(Server, Reliable)
    void ServerReadyUp();

protected:
    int readyPlayers;

    int numOfEnemies;

    FTimerHandle TimerHandle_SpawnEnemies;

    void StartWave();

    void EndWave();

    void SpawnEnemiesTimer();

    UFUNCTION(BlueprintImplementableEvent)
    void SpawnEnemies();
};
