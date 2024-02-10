// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyProject/AbilitySystem/BaseAbilities/Public/EquipAbilityBase.h"
#include "SwapAbility.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API USwapAbility : public UEquipAbilityBase
{
	GENERATED_BODY()

public:
	virtual bool Activate(ABaseCharacter* Character) override;
	
	virtual bool EndActivation() override;

	virtual void UseMainAction() override;

};
