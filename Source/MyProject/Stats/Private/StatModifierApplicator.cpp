// Fill out your copyright notice in the Description page of Project Settings.


#include "MyProject/Stats/Public/StatModifierApplicator.h"

#include "MyProject/Characters/Public/BaseCharacter.h"
#include "MyProject/Stats/Enums/Public/StatEnum.h"
#include "MyProject/Stats/StatModifiers/Public/StatModifierTuple.h"

void UStatModifierApplicator::ApplyStatModifiers(ABaseCharacter* Character)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("apply stats called")));
	for (UStatModifierTuple* StatModifierTuple : StatModifierTuples)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("applying")));
		ApplyStatModifier(Character, StatModifierTuple);
	}
}

void UStatModifierApplicator::ApplyStatModifier(ABaseCharacter* Character, UStatModifierTuple* StatModifierTuple)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("apply called")));
	UClass* TypeClass = StatModifierTuple->GetStatModifierClass();
	
	if (UStatModifier* StatModifier = NewObject<UStatModifier>(this, StatModifierTuple->GetStatModifierClass()))
	{
	    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Stat %f"), StatModifierTuple->GetModifierValue()));
		StatModifier->SetModifier(StatModifierTuple->GetModifierValue());

		switch (StatModifierTuple->GetStatEnum())
		{
		case EStatEnum::SE_MovementSpeed:
			Character->AddMovementSpeedModifier(StatModifier);
			break;
		}
	}
}
