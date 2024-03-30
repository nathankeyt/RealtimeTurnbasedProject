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

	// Abilities.Init(nullptr, 10);

	// ActiveAbilities.Init(false, 10);
}


// Called when the game starts
void UAbilitySystemComponent::BeginPlay()
{ 
	Super::BeginPlay();

	InitializeUI();

	for (int i = 0; i < Abilities.Num(); i++)
	{
		Abilities[i] = DuplicateObject(Abilities[i], nullptr);
	}

	// GetWorld()->OnWorldBeginPlay.AddUObject(this, &ThisClass::InitializeUI);
}

void UAbilitySystemComponent::InitializeUI()
{
	if (AbilityMenuWidget != nullptr)
	{
		AbilityMenuWidget->Initialize();
		for (UAbility* Ability : Abilities)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("adding ability to list")));
			AbilityMenuWidget->AddAbilityToList(Ability);
		}
	}
}



// Called every frame
void UAbilitySystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/*
	for (UAbility* Ability : Abilities)
	{
		if (Ability->GetIsActive())
		{
			Ability->WhileActive();
		}

		if (Ability->GetIsMainActive())
		{
			Ability->WhileMainActive();
		}
	}*/
}

void UAbilitySystemComponent::ActivateAbility_Implementation(int AbilityIndex, ABaseCharacter* Character)
{
	if (AbilityIndex >= 0 && AbilityIndex < Abilities.Num() && Abilities[AbilityIndex] != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("ability activation or deactivation moving forward")));
		const EAbilityActivationType AbilityActivationType = Abilities[AbilityIndex]->GetAbilityActivationType();
 
		if (AbilityActivationType == EAbilityActivationType::AA_Toggleable && Abilities[AbilityIndex]->GetIsActive())
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("ability end activation attempt")));
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
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("main equipped ability not null")));
			if (MainEquippedAbility->EndActivation())
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("main equipped ability deactivated")));
				MainEquippedAbility = nullptr;
				if (Ability->Activate(Character))
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("ability activated")));
					MainEquippedAbility = Ability;
				}
			}
		}
		else
		{
			if (Ability->Activate(Character))
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("ability activated")));
				MainEquippedAbility = Ability;
			}
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("ability activated")));
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
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("ability end activation forward")));
		const EAbilityType AbilityType = Ability->GetAbilityType();
		
		if (AbilityType == EAbilityType::AT_MainEquippedAbility) {
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("ability end activation")));
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

		if (AbilityMenuWidget != nullptr)
		{
			AbilityMenuWidget->AddAbilityToList(Ability);
		}
	}
}

void UAbilitySystemComponent::DisplayAbilityMenu()
{
	if (AbilityMenuWidget != nullptr)
	{
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



