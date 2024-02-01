// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability.h"
#include "EquipAbilityBase.h"
#include "EnergyShot.generated.h"

class UNiagaraComponent;
class UProjectileSpawner;
class UNiagaraSystem;
/**
 * 
 */
UCLASS()
class MYPROJECT_API UEnergyShot : public UEquipAbilityBase
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
	virtual bool Activate(ABaseCharacter* Character) override;

	virtual bool EndActivation() override;

	virtual void Charge() override;

	virtual void Fire() override;
};
