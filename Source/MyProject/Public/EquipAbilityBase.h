// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability.h"
#include "EquipAbilityBase.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UEquipAbilityBase : public UAbility
{
	GENERATED_BODY()

public:
	virtual void Charge() {};

	virtual void Fire() {};
};
