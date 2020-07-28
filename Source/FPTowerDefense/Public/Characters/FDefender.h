// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FCharacter.h"
#include "FDefender.generated.h"

/**
 * 
 */
UCLASS()
class FPTOWERDEFENSE_API AFDefender : public AFCharacter
{
	GENERATED_BODY()
	
public:


protected:

	virtual void UseOffensive() override;

	virtual void UseSupport() override;

};
