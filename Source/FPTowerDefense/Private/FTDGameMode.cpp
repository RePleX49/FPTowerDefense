// Fill out your copyright notice in the Description page of Project Settings.


#include "FTDGameMode.h"
#include "FTDGameState.h"

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
	// todo confirm that player is not already ready to increment
	readyPlayers++;

	UpdateReadyPlayerCount();

	if (readyPlayers == GetNumPlayers())
	{
		// call to start the wave
		AFTDGameState* MyGameState = Cast<AFTDGameState>(GameState);
		if (MyGameState)
		{
			MyGameState->isMatchReady = true;
			MyGameState->OnRep_MatchReady();
		}
		StartWave();
	}
}

void AFTDGameMode::PlayerUnready()
{
	readyPlayers--;
	UpdateReadyPlayerCount();
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

void AFTDGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	AFTDGameState* MyGameState = Cast<AFTDGameState>(GameState);
	if (MyGameState)
	{
		MyGameState->PlayersInMatch++;
	}
}

void AFTDGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	AFTDGameState* MyGameState = Cast<AFTDGameState>(GameState);
	if (MyGameState)
	{
		MyGameState->PlayersInMatch--;
	}
}

void AFTDGameMode::StartWave()
{
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnEnemies, this, &AFTDGameMode::SpawnEnemiesTimer, 0.5f, true, 0.0f);
}

void AFTDGameMode::EndWave()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_SpawnEnemies);
}

void AFTDGameMode::UpdateReadyPlayerCount()
{
	AFTDGameState* MyGameState = Cast<AFTDGameState>(GameState);
	if (MyGameState)
	{
		MyGameState->NumPlayersReady = readyPlayers;
	}
}
