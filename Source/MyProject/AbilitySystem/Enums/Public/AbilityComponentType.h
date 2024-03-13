#pragma once

#include "CoreMinimal.h"
#include "AbilityComponentType.generated.h"

UENUM(BlueprintType)
enum class EAbilityComponentType : uint8
{
	AC_OnActivate	UMETA(DisplayName = "OnActivate"),
	AC_OnRelease	UMETA(DisplayName = "OnRelease"),
	AC_WhileActive UMETA(DisplayName = "WhileActive"),
	AC_OnHit UMETA(DisplayName = "OnHit"),
};