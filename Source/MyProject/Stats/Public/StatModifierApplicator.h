// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Templates/Tuple.h"
#include "StatModifierApplicator.generated.h"

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
	TArray<UStatModifierTuple*> StatModifierTuples;

public:
	void ApplyStatModifiers(ABaseCharacter* Character);

	void ApplyStatModifier(ABaseCharacter* Character, UStatModifierTuple* StatModifierTuple);
};
