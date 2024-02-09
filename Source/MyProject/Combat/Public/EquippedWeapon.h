// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Engine/DataAsset.h"
#include "EquippedWeapon.generated.h"

class ABaseCharacter;
enum class EAttackLevelEnum : uint8;
class UAttackNode;
class UAttackWrapperNode;
class UComboNode;
class UNiagaraSystem;
enum class EKnockBackEnum : uint8;
class UStat;
/**
 * 
 */
UCLASS(Blueprintable, EditInlineNew)
class MYPROJECT_API UEquippedWeapon : public UWeapon
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CombatMontages )
	UComboNode* Attacks;
	
	UPROPERTY(BlueprintReadOnly, Category = CombatMontages )
	int ComboCounter = 0;

	UPROPERTY(BlueprintReadOnly, Category = CombatMontages )
	UComboNode* CurrComboNode;

	UPROPERTY(BlueprintReadOnly, Category = CombatMontages )
	UAttackWrapperNode* CurrAttackWrapperNode;

	UPROPERTY(BlueprintReadOnly, Category = CombatMontages )
	UAttackNode* CurrAttackNode;

	UPROPERTY(BlueprintReadOnly, Category = Timer )
	bool ShouldComboReset = false;

	UPROPERTY(BlueprintReadOnly, Category = Timer )
	FTimerHandle ComboResetTimeHandle;


public:
	UAnimMontage* GetNextAttackMontage(const ABaseCharacter* Character, const bool IsAltAttack, const EAttackLevelEnum AttackLevel, const bool ShouldMove);

	void ResetCombo();
	
};
