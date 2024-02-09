// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AttackWrapperNode.generated.h"

class UAttackNode;
/**
 * 
 */
UCLASS(Blueprintable, EditInlineNew)
class MYPROJECT_API UAttackWrapperNode : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=AttackNodes)
	UAttackNode* LightAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=AttackNodes)
	UAttackNode* HeavyAttack;

};
