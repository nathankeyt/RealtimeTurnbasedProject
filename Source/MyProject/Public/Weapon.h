// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Weapon.generated.h"

class UStat;
/**
 * 
 */
UCLASS(Blueprintable, EditInlineNew)
class MYPROJECT_API UWeapon : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	UStat* Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	UStat* HeavyDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	UStat* StaminaDamage;

public:
	
	UStat* GetDamage() const { return Damage; }

	UStat* GetHeavyDamage() const { return HeavyDamage; }

	UStat* GetStaminaDamage() const { return StaminaDamage; }

	void SetDamage(UStat* Dmg) { Damage = Dmg; }

	void SetHeavyDamage(UStat* HeavyDmg) { HeavyDamage = HeavyDmg; }

	void SetStaminaDamage(UStat* StaminaDmg) { StaminaDamage = StaminaDmg; }
	
};
