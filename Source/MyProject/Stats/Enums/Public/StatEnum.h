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
	SE_Misc			UMETA(DisplayName = "Misc"),
	SE_Health			UMETA(DisplayName = "Health"),
	SE_MaxHealth			UMETA(DisplayName = "MaxHealth"),
	SE_Mana				UMETA(DisplayName = "Mana"),
	SE_MaxMana		UMETA(DisplayName = "MaxMana"),
	SE_Stamina			UMETA(DisplayName = "Stamina"),
	SE_MaxStamina	UMETA(DisplayName = "MaxStamina"),
	SE_MovementSpeed	UMETA(DisplayName = "MovementSpeed") 
};
