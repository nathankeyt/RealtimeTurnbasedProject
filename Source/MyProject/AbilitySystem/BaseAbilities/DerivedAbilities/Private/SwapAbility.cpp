// Fill out your copyright notice in the Description page of Project Settings.


#include "MyProject/AbilitySystem/BaseAbilities/DerivedAbilities/Public/SwapAbility.h"

#include "MyProject/Characters/Player/Public/PlayerCharacter.h"

bool USwapAbility::Activate(ABaseCharacter* Character)
{
	ActiveCharacter = Character;
	IsActive = true;
		
	return true;
}

bool USwapAbility::EndActivation()
{
	if (ActiveCharacter != nullptr)
	{
		ActiveCharacter = nullptr;
		IsActive = false;
		
		return true;
	}

	return false;
}

void USwapAbility::UseMainAction()
{
	
}


