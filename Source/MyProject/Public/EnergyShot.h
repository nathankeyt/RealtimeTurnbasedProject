// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability.h"
#include "EnergyShot.generated.h"

class UNiagaraComponent;
class UProjectileSpawner;
class UNiagaraSystem;
/**
 * 
 */
UCLASS()
class MYPROJECT_API UEnergyShot : public UAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraSystem* ChargeEffectTemplate;

	UPROPERTY(BlueprintReadWrite)
	UNiagaraComponent* ActiveChargeEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	UProjectileSpawner* ProjectileSpawner;
	
public:
	virtual void Activate(ABaseCharacter* Character) override;

	virtual void EndActivation() override;
};
