// Fill out your copyright notice in the Description page of Project Settings.


#include "FTDGameState.h"
#include "Net/UnrealNetwork.h"

// Remember to include "Net/UnrealNetwork.h" for this
void AFTDGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFTDGameState, NumPlayersReady);
	DOREPLIFETIME(AFTDGameState, PlayersInMatch);
	DOREPLIFETIME(AFTDGameState, isMatchReady);
}