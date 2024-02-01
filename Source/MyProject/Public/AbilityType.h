#pragma once

#include "CoreMinimal.h"
#include "AbilityType.generated.h"

UENUM(BlueprintType)
enum class EAbilityType : uint8
{
	AT_BasicAbility	UMETA(DisplayName = "BasicAbility"),
	AT_MainEquippedAbility	UMETA(DisplayName = "MainEquippedAbility")
};