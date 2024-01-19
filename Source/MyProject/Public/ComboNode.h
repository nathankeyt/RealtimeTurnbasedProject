// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ComboNode.generated.h"

/**
 * 
 */
UCLASS(Blueprintable,EditInlineNew)
class MYPROJECT_API UComboNode : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=ComboMontages)
	UAnimMontage* CurrMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced, Category=ComboMontages)
	UComboNode* Next;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced, Category=ComboMontages)
	UComboNode* End;
};
