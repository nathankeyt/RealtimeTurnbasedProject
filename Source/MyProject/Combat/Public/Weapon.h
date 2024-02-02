// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Weapon.generated.h"

enum class EKnockBackEnum : uint8;
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EKnockBackEnum KnockBackEnum = EKnockBackEnum::KE_NoKnockBack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EKnockBackEnum HeavyKnockBackEnum = EKnockBackEnum::KE_LightKnockBack;

public:
	
	UStat* GetDamage() const { return Damage; }

	UStat* GetHeavyDamage() const { return HeavyDamage; }

	UStat* GetStaminaDamage() const { return StaminaDamage; }

	EKnockBackEnum GetKnockBack() const { return KnockBackEnum; }

	EKnockBackEnum GetHeavyKnockBack() const { return HeavyKnockBackEnum; }

	void SetDamage(UStat* Dmg) { Damage = Dmg; }

	void SetHeavyDamage(UStat* HeavyDmg) { HeavyDamage = HeavyDmg; }

	void SetStaminaDamage(UStat* StaminaDmg) { StaminaDamage = StaminaDmg; }

	void SetKnockBack(EKnockBackEnum KnockBack) { KnockBackEnum = KnockBack; }

	void SetHeavyKnockBack(EKnockBackEnum KnockBack) { HeavyKnockBackEnum = KnockBack; }
	
};
