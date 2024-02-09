// Fill out your copyright notice in the Description page of Project Settings.


#include "MyProject/Combat/Public/EquippedWeapon.h"

#include "MyProject/Characters/Public/BaseCharacter.h"
#include "MyProject/Combat/Enums/Public/AttackLevelEnum.h"
#include "MyProject/Combat/Public/AttackNode.h"
#include "MyProject/Combat/Public/AttackWrapperNode.h"
#include "MyProject/Combat/Public/ComboNode.h"

UAnimMontage* UEquippedWeapon::GetNextAttackMontage(const ABaseCharacter* Character, const bool IsAltAttack, const EAttackLevelEnum AttackLevel, const bool ShouldMove)
{
	if (Character != nullptr && Attacks != nullptr)
	{
		Character->GetWorldTimerManager().SetTimer(ComboResetTimeHandle, this, &UEquippedWeapon::ResetCombo, 1.0f);
		
		do
		{
			if (CurrComboNode == nullptr || ShouldComboReset)
			{
				CurrComboNode = Attacks;
				ShouldComboReset = false;
			}
			
			if (IsAltAttack)
			{
				CurrAttackWrapperNode = CurrComboNode->AltAttack;
			}
			else
			{
				CurrAttackWrapperNode = CurrComboNode->MainAttack;
			}

			if (CurrAttackWrapperNode != nullptr)
			{
				if (AttackLevel == EAttackLevelEnum::AE_LightAttack)
				{
					CurrAttackNode = CurrAttackWrapperNode->LightAttack;
				}
				else if (AttackLevel == EAttackLevelEnum::AE_HeavyAttack)
				{
					CurrAttackNode = CurrAttackWrapperNode->HeavyAttack;
				}

				if (CurrAttackNode != nullptr)
				{
					CurrComboNode = CurrAttackNode->Next;
				}
				else
				{
					ShouldComboReset = true;
				}
			}
			else
			{
				ShouldComboReset = true;
			}
		}
		while (ShouldComboReset);

		if (CurrAttackNode != nullptr)
		{
			if (ShouldMove)
			{
				return CurrAttackNode->MontageM;
			}

			return CurrAttackNode->MontageIP;
		}
		
	}

	return nullptr;
}

void UEquippedWeapon::ResetCombo()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("combo reset timer called")));
	ShouldComboReset = true;
}


