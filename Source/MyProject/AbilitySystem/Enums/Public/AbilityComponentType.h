#pragma once

#include "CoreMinimal.h"
#include "AbilityComponentType.generated.h"

UENUM(BlueprintType)
enum class EAbilityComponentType : uint8
{
	AC_OnActivate	UMETA(DisplayName = "BasicAbility"),
	AC_OnRelease	UMETA(DisplayName = "BasicAbility"),
	AC_WhileActive UMETA(DisplayName = "MainEquippedAbility"),
	AC_OnHit UMETA(DisplayName = "MainEquippedAbility"),
};