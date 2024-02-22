// Fill out your copyright notice in the Description page of Project Settings.


#include "MyProject/AbilitySystem/UI/Public/AbilityMenuWidget.h"

#include "Components/ListView.h"
#include "MyProject/AbilitySystem/BaseAbilities/Public/Ability.h"
#include "MyProject/AbilitySystem/UI/Public/AbilityEntryWidget.h"

void UAbilityMenuWidget::AddAbilityToList(UAbility* Ability) const
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("adding ability to ability menu listview 1")));
	if (Ability != nullptr && AbilityListView != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("adding ability to ability menu listview 2")));
		if (UAbilityEntryWidget* AbilityEntryWidget = Ability->GetAbilityEntryWidget())
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("adding ability to ability menu listview 3")));
			AbilityListView->AddItem(AbilityEntryWidget);
		}
	}
}
