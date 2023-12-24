// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Ability.generated.h"

class UStatModifierApplicator;

/**
 * 
 */
UCLASS(Abstract, Blueprintable, EditInlineNew)
class MYPROJECT_API UAbility : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UTexture2D* AbilityDisplayImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	UStatModifierApplicator* StatModifierApplicator;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int ManaCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int ActivationSpeed;

public:
	UFUNCTION(BlueprintCallable)
	virtual void Activate(ABaseCharacter* Character) PURE_VIRTUAL(UAbility::Activate(), );

	UFUNCTION(BlueprintCallable)
	UTexture2D* GetAbilityDisplayImage() const { return AbilityDisplayImage; }
};
