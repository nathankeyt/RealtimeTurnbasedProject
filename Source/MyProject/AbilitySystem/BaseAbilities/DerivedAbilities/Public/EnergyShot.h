// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyProject/AbilitySystem/BaseAbilities/Public/Ability.h"
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
	virtual bool Activate(ABaseCharacter* Character) override;

	virtual bool EndActivation() override;

	virtual void UseMainAction() override;

	virtual void EndMainAction() override;



};
