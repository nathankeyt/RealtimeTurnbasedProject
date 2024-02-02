// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyProject/AbilitySystem/BaseAbilities/Public/Ability.h"
#include "FireAbility.generated.h"

class UProjectileSpawner;
/**
 * 
 */
UCLASS()
class MYPROJECT_API UFireAbility : public UAbility
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	UProjectileSpawner* ProjectileSpawner;
	
public:
	virtual bool Activate(ABaseCharacter* Character) override;

	virtual bool EndActivation() override;
};
