// Fill out your copyright notice in the Description page of Project Settings.


#include "MyProject/AbilitySystem/UI/Public/AbilityEntryWidget.h"

#include "Components/TextBlock.h"
#include "MyProject/AbilitySystem/BaseAbilities/Public/Ability.h"

void UAbilityEntryWidget::SetAbility(UAbility* Ability)
{
	if (Ability != nullptr && AbilityName != nullptr)
	{
		AbilityName->SetText(Ability->AbilityName);
	}
}

