// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyProject/AbilitySystem/BaseAbilities/Public/Ability.h"
#include "DragonsBreath.generated.h"

class UNiagaraComponent;
class UProjectileSpawner;
class UNiagaraSystem;
/**
 * 
 */
UCLASS()
class MYPROJECT_API UDragonsBreath : public UAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	UProjectileSpawner* ProjectileSpawner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTimerHandle BreathHandle;
	
public:
	virtual bool Activate(ABaseCharacter* Character) override;

	virtual bool EndActivation() override;

	virtual void UseMainAction() override;

	virtual void EndMainAction() override;

	virtual void WhileMainActive() override;

};
