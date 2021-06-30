// Fill out your copyright notice in the Description page of Project Settings.


#include "FTDGameMode.h"

AFTDGameMode::AFTDGameMode()
{
	readyPlayers = 0;
}

void AFTDGameMode::StartPlay()
{
	Super::StartPlay();
}

void AFTDGameMode::PlayerReadyUp()
{
	if (GetLocalRole() < ROLE_Authority)
	{
		ServerReadyUp();
	}

	readyPlayers++;

	if (readyPlayers == GetNumPlayers())
	{
		// call to start the wave
		StartWave();
	}
}

void AFTDGameMode::ServerReadyUp_Implementation()
{
	PlayerReadyUp();
}

void AFTDGameMode::SpawnEnemiesTimer()
{
	SpawnEnemies();

	numOfEnemies--;

	if (numOfEnemies <= 0)
	{
		EndWave();
	}
}

void AFTDGameMode::StartWave()
{
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnEnemies, this, &AFTDGameMode::SpawnEnemiesTimer, 1.0f, true, 0.0f);
}

void AFTDGameMode::EndWave()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_SpawnEnemies);
}