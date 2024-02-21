// Fill out your copyright notice in the Description page of Project Settings.


#include "MyProject/AbilitySystem/UI/Public/AbilityMenuWidget.h"

#include "Components/ListView.h"
#include "MyProject/AbilitySystem/BaseAbilities/Public/Ability.h"
#include "MyProject/AbilitySystem/UI/Public/AbilityEntryWidget.h"

void UAbilityMenuWidget::AddAbilityToList(UAbility* Ability)
{
	if (Ability != nullptr && AbilityListView != nullptr)
	{
		if (UAbilityEntryWidget* AbilityEntryWidget = Ability->GetAbilityEntryWidget())
		{
			AbilityListView->AddItem(AbilityEntryWidget);
		}
	}
}
