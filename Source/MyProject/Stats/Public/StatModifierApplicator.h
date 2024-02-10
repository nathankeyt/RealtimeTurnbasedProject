// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyProject/Stats/Enums/Public/StatEnum.h"
#include "Templates/Tuple.h"
#include "StatModifierApplicator.generated.h"

enum class EStatEnum : uint8;
class ABaseCharacter;
class UStatModifierTuple;
class UStatModifier;
/**
 * 
 */
UCLASS(Blueprintable, EditInlineNew)
class MYPROJECT_API UStatModifierApplicator : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	TMap<EStatEnum, UStatModifierTuple*> StatModifierMap;

public:
	void ApplyStatModifiersToCharacter(ABaseCharacter* Character);

	void ApplyStatModifierToCharacter(ABaseCharacter* Character, EStatEnum StatType, UStatModifierTuple* StatModifierTuple);

	void ApplyStatModifiersToStatApplicator(UStatModifierApplicator* StatModifierApplicator);

	void ApplyStatModifierToStatApplicator(UStatModifierApplicator* OtherStatModifierApplicator, EStatEnum StatType, UStatModifierTuple* StatModifierTuple);
};
