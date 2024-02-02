// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyProject/AbilitySystem/BaseAbilities/Public/Ability.h"
#include "FlamingFists.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;
class UProjectileSpawner;
/**
 * 
 */
UCLASS()
class MYPROJECT_API UFlamingFists : public UAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraSystem* NiagaraEffectTemplate;

	UPROPERTY(BlueprintReadWrite)
	TArray<UNiagaraComponent*> ActiveNiagaraEffects;

public:
	virtual bool Activate(ABaseCharacter* Character) override;

	virtual bool EndActivation() override;
};
