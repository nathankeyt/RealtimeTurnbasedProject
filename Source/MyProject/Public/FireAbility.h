// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability.h"
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
	virtual void Activate(ABaseCharacter* Character) override;

	virtual void EndActivation() override;
};
