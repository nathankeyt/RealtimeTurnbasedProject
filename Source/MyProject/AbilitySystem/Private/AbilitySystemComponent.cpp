// Fill out your copyright notice in the Description page of Project Settings.


#include "MyProject/AbilitySystem/Public/AbilitySystemComponent.h"

#include "MyProject/AbilitySystem/BaseAbilities/Public/Ability.h"
#include "MyProject/AbilitySystem/Enums/Public/AbilityActivationType.h"
#include "MyProject/AbilitySystem/Enums/Public/AbilityType.h"
#include "MyProject/AbilitySystem/UI/Public/AbilityMenuWidget.h"
// Sets default values for this component's properties
UAbilitySystemComponent::UAbilitySystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	SetIsReplicatedByDefault(true);

	//Abilities.Init(nullptr, 10);

	//ActiveAbilities.Init(false, 10);
}


// Called when the game starts
void UAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();

	if (AbilityMenuWidget != nullptr)
	{
		for (UAbility* Ability : Abilities)
		{
			ActiveAbilities.Add(false);
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Adding Ability to Menu")));
			AbilityMenuWidget->AddAbilityToList(Ability);
		}
	}
	

	// ...
	
}


// Called every frame
void UAbilitySystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAbilitySystemComponent::ActivateAbility_Implementation(int AbilityIndex, ABaseCharacter* Character)
{
	if (AbilityIndex >= 0 && AbilityIndex < Abilities.Num() && Abilities[AbilityIndex] != nullptr)
	{
		const EAbilityActivationType AbilityActivationType = Abilities[AbilityIndex]->GetAbilityActivationType();
 
		if (AbilityActivationType == EAbilityActivationType::AA_Toggleable && Abilities[AbilityIndex]->GetIsActive())
		{
			EndAbilityActivationByType(Abilities[AbilityIndex]);
		}
		else
		{
			ActivateAbilityByType(Abilities[AbilityIndex], Character);
		}
	}
}

void UAbilitySystemComponent::ActivateAbilityByType(UAbility* Ability, ABaseCharacter* Character)
{
	const EAbilityType AbilityType = Ability->GetAbilityType();
	
	if (AbilityType == EAbilityType::AT_MainEquippedAbility)
	{
		if (MainEquippedAbility != nullptr) {
			if (MainEquippedAbility->EndActivation())
			{
				MainEquippedAbility = nullptr;
				if (Ability->Activate(Character))
				{
					MainEquippedAbility = Ability;
				}
			}
		}
		else
		{
			if (Ability->Activate(Character))
			{
				MainEquippedAbility = Ability;
			}
		}
	}
	else
	{
		Ability->Activate(Character);
	}
}


void UAbilitySystemComponent::EndAbilityActivation_Implementation(int AbilityIndex)
{
	if (AbilityIndex >= 0 && AbilityIndex < Abilities.Num() && Abilities[AbilityIndex] != nullptr && Abilities[AbilityIndex]->GetAbilityActivationType() == EAbilityActivationType::AA_HoldAndRelease)
	{
		EndAbilityActivationByType(Abilities[AbilityIndex]);
	}
}

void UAbilitySystemComponent::EndAbilityActivationByType(UAbility* Ability)
{
	if (Ability->EndActivation())
	{
		const EAbilityType AbilityType = Ability->GetAbilityType();
		
		if (AbilityType == EAbilityType::AT_MainEquippedAbility) {
			MainEquippedAbility = nullptr;
		}
	}
}


UAbility* UAbilitySystemComponent::GetMainEquippedAbility()
{
	return MainEquippedAbility;
}

void UAbilitySystemComponent::AddAbility(UAbility* Ability)
{
	if (Ability != nullptr)
	{
		Abilities.Add(Ability);
		ActiveAbilities.Add(false);

		if (AbilityMenuWidget != nullptr)
		{
			AbilityMenuWidget->AddAbilityToList(Ability);
		}
	}
}

void UAbilitySystemComponent::DisplayAbilityMenu()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("in displaying ability menu func")));
	if (AbilityMenuWidget != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("displaying ability menu")));
		AbilityMenuWidget->AddToViewport();
	}
}

void UAbilitySystemComponent::HideAbilityMenu()
{
	if (AbilityMenuWidget != nullptr)
	{
		AbilityMenuWidget->RemoveFromParent();
	}
}



