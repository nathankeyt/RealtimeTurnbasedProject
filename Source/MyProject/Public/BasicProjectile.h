// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability.h"
#include "BasicProjectile.generated.h"

class UProjectileSpawner;
class ABaseCharacter;

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
	virtual bool Activate(ABaseCharacter* Character) override;
};
