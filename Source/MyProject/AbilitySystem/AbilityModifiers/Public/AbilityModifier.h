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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStatModifierApplicator* StatModifierApplicator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText DisplayText;
public:
	virtual void ApplyModifier(UAbility* Ability) {};
};
