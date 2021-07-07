// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "FTDGameState.generated.h"

/**
 * 
 */
UCLASS()
class FPTOWERDEFENSE_API AFTDGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Replicated, BlueprintReadOnly)
	int32 NumPlayersReady;

	UPROPERTY(Replicated, BlueprintReadWrite)
	int32 PlayersInMatch;

	UPROPERTY(ReplicatedUsing = OnRep_MatchReady, BlueprintReadOnly);
	bool isMatchReady;

	//need to manually call for listen server
	UFUNCTION(BlueprintImplementableEvent)
	void OnRep_MatchReady();
};
