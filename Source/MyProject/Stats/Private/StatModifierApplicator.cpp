// Fill out your copyright notice in the Description page of Project Settings.


#include "MyProject/Stats/Public/StatModifierApplicator.h"

#include "MyProject/Characters/Public/BaseCharacter.h"
#include "MyProject/Stats/Enums/Public/StatEnum.h"
#include "MyProject/Stats/StatModifiers/Public/StatModifier.h"
#include "MyProject/Stats/StatModifiers/Public/StatModifierTuple.h"

void UStatModifierApplicator::ApplyStatModifiersToCharacter(ABaseCharacter* Character)
{
	for (const TPair<EStatEnum, UStatModifierTuple*>& Pair : StatModifierMap)
	{
		ApplyStatModifierToCharacter(Character, Pair.Key, Pair.Value);
	}
}

void UStatModifierApplicator::ApplyStatModifierToCharacter(ABaseCharacter* Character, EStatEnum StatType, UStatModifierTuple* StatModifierTuple)
{
	if (StatModifierTuple != nullptr)
	{
		if (UStatModifier* StatModifier = NewObject<UStatModifier>(this, StatModifierTuple->GetStatModifierClass()))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Stat %f"), StatModifierTuple->GetStatModifierValue()));
			StatModifier->SetModifier(StatModifierTuple->GetStatModifierValue());

			switch (StatType)
			{
			case EStatEnum::SE_MovementSpeed:
				Character->AddMovementSpeedModifier(StatModifier);
				break;
			default: ;
			}
		}
	}
}

void UStatModifierApplicator::ApplyStatModifiersToStatApplicator(UStatModifierApplicator* StatModifierApplicator)
{
	for (const TPair<EStatEnum, UStatModifierTuple*>& Pair : StatModifierMap)
	{
		ApplyStatModifierToStatApplicator(StatModifierApplicator, Pair.Key, Pair.Value);
	}
}

void UStatModifierApplicator::ApplyStatModifierToStatApplicator(UStatModifierApplicator* OtherStatModifierApplicator, EStatEnum StatType, UStatModifierTuple* StatModifierTuple)
{
	if (StatModifierTuple != nullptr)
	{
		if (UStatModifier* StatModifier = NewObject<UStatModifier>(this, StatModifierTuple->GetStatModifierClass()))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Stat %f"), StatModifierTuple->GetStatModifierValue()));
			StatModifier->SetModifier(StatModifierTuple->GetStatModifierValue());

			UStatModifierTuple* OtherTuple = OtherStatModifierApplicator->StatModifierMap[StatType];
			
			if (OtherTuple != nullptr && OtherTuple->GetStatModifier() != nullptr)
			{
				StatModifier->SetParentStat(OtherTuple->GetStatModifier());
				OtherTuple->SetStatModifier(StatModifier);
			}
		}
	}
}

