// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability.h"
#include "ProjectileSpawner.h"
#include "StatModifierApplicator.h"
#include "BasicProjectile.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UBasicProjectile : public UAbility
{
	GENERATED_BODY()

public:
	UBasicProjectile();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	UProjectileSpawner* ProjectileSpawner;

public:
	virtual void Activate(ABaseCharacter* Character) override;
};
