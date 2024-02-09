// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AttackNode.generated.h"

class UComboNode;
/**
 * 
 */
UCLASS(Blueprintable, EditInlineNew)
class MYPROJECT_API UAttackNode : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=ComboMontages)
	UAnimMontage* MontageIP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=ComboMontages)
	UAnimMontage* MontageM;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=ComboMontages)
	UComboNode* Next;
};
