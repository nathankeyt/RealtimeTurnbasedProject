#pragma once

#include "CoreMinimal.h"
#include "AbilityActivationType.generated.h"

UENUM(BlueprintType)
enum class EAbilityActivationType : uint8
{
	AA_HoldAndRelease	UMETA(DisplayName = "HoldAndRelease"),
	AA_Toggleable	UMETA(DisplayName = "Toggleable")
};