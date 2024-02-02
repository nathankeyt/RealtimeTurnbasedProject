// Fill out your copyright notice in the Description page of Project Settings.


#include "MyProject/AbilitySystem/UI/Public/AbilityDisplayWidget.h"

#include "Components/Image.h"
#include "Kismet/KismetMathLibrary.h"
#include "MyProject/AbilitySystem/BaseAbilities/Public/Ability.h"
#include "MyProject/AbilitySystem/Public/AbilitySystemComponent.h"

void UAbilityDisplayWidget::SetAbilityDisplay(UAbilitySystemComponent* AbilitySystem)
{
	if (AbilitySystem != nullptr)
	{
		const TArray<UAbility*> Abilities = AbilitySystem->GetAbilities();
		
		for (int i = 0; i < UKismetMathLibrary::Min(Abilities.Num(), AbilityDisplays.Num()); i++) {

			const UAbility* Ability = Abilities[i];
			
			if (Ability != nullptr)
			{
				UTexture2D* AbilityDisplayImage = Ability->GetAbilityDisplayImage();
			
				if (AbilityDisplayImage != nullptr)
				{
					AbilityDisplays[i]->SetBrushFromTexture(AbilityDisplayImage);
				}
			}
		}
	}
}
