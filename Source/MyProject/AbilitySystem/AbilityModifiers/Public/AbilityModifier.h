#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AbilityModifier.generated.h"

class UStatModifierApplicator;
class UAbility;
/**
 * 
 */
UCLASS(Abstract, Blueprintable, EditInlineNew)
class MYPROJECT_API  UAbilityModifier : public UDataAsset
{
	GENERATED_BODY()
public:
	UStatModifierApplicator* StatModifierApplicator;
public:
	virtual void ApplyModifier(UAbility* Ability) {};
};
