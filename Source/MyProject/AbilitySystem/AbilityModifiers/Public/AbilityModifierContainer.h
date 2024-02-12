#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AbilityModifierContainer.generated.h"

class UAbilityModifier;
class UStatModifierApplicator;
class UAbility;
/**
 * 
 */
UCLASS(Abstract, Blueprintable, EditInlineNew)
class MYPROJECT_API  UAbilityModifierContainer : public UDataAsset
{
	GENERATED_BODY()
public:
	TArray<UAbilityModifier*> AbilityModifiers;
};
