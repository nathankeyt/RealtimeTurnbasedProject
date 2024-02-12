// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyProject/Combat/Enums/Public/KnockbackEnum.h"
#include "Engine/DataAsset.h"
#include "MyProject/Stats/Enums/Public/StatEnum.h"
#include "Weapon.generated.h"

class UNiagaraSystem;
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
	TMap<EStatEnum, UStat*> StatMap;

	/*
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	UStat* Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	UStat* HeavyDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	UStat* StaminaDamage;
*/
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EKnockBackEnum KnockBackEnum = EKnockBackEnum::KE_NoKnockBack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EKnockBackEnum HeavyKnockBackEnum = EKnockBackEnum::KE_LightKnockBack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraSystem* HitEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HitParticleSize = 1.0f;
public:
	
	UStat* GetLightDamage() { return StatMap.FindOrAdd(EStatEnum::SE_LightDamage); }

	UStat* GetHeavyDamage() { return StatMap.FindOrAdd(EStatEnum::SE_HeavyDamage); }

	UStat* GetStaminaDamage() { return StatMap.FindOrAdd(EStatEnum::SE_StaminaDamage); }

	EKnockBackEnum GetKnockBack() const { return KnockBackEnum; }

	EKnockBackEnum GetHeavyKnockBack() const { return HeavyKnockBackEnum; }

	void SetLightDamage(UStat* Dmg) { StatMap.FindOrAdd(EStatEnum::SE_LightDamage) = Dmg; }

	void SetHeavyDamage(UStat* HeavyDmg) { StatMap.FindOrAdd(EStatEnum::SE_HeavyDamage) = HeavyDmg; }

	void SetStaminaDamage(UStat* StaminaDmg) { StatMap.FindOrAdd(EStatEnum::SE_StaminaDamage) = StaminaDmg; }

	void SetKnockBack(const EKnockBackEnum KnockBack) { KnockBackEnum = KnockBack; }

	void SetHeavyKnockBack(const EKnockBackEnum KnockBack) { HeavyKnockBackEnum = KnockBack; }
	
};
