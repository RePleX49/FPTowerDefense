// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FTDGameMode.generated.h"

UCLASS()
class FPTOWERDEFENSE_API AFTDGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
    AFTDGameMode();

    virtual void StartPlay() override;

    UFUNCTION(BlueprintCallable)
    void PlayerReadyUp();

    UFUNCTION(BlueprintCallable)
    void PlayerUnready();

protected:
    int readyPlayers;

    UPROPERTY(EditDefaultsOnly, Category = "Properties")
    int numOfEnemies;

    FTimerHandle TimerHandle_SpawnEnemies;

    void StartWave();

    void EndWave();

    void UpdateReadyPlayerCount();

    void SpawnEnemiesTimer();

    UFUNCTION(BlueprintImplementableEvent)
    void SpawnEnemies();

    virtual void PostLogin(APlayerController* NewPlayer) override;

    virtual void Logout(AController* Exiting) override;
};
