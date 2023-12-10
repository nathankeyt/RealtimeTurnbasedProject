// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatEnum.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EStatEnum : uint8
{
	SE_Health			UMETA(DisplayName = "Health"),
	SE_Mana				UMETA(DisplayName = "Mana"),
	SE_MovementSpeed	UMETA(DisplayName = "MovementSpeed") 
};
