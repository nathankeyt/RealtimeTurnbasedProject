// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TurnEnum.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ETurnEnum : uint8
{
		TE_PlayerTurn	UMETA(DisplayName = "PlayerTurn"),
		TE_EnemyTurn	UMETA(DisplayName = "EnemyTurn")
};
