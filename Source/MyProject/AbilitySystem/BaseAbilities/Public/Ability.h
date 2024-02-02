// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Ability.generated.h"

enum class EAbilityActivationType : uint8;
enum class EAbilityType : uint8;
class ABaseCharacter;
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EAbilityType AbilityType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EAbilityActivationType AbilityActivationType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ABaseCharacter* ActiveCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsActive;

public:
	UFUNCTION(BlueprintCallable)
	virtual bool Activate(ABaseCharacter* Character) { return false; };

	UFUNCTION(BlueprintCallable)
	virtual bool EndActivation() { return false; };

	virtual void UseMainAction() {};

	virtual void EndMainAction() {};

	virtual void UseAltAction() {};

	virtual void EndAltAction() {};

	UFUNCTION(BlueprintCallable)
	EAbilityType GetAbilityType() const { return AbilityType; }

	UFUNCTION(BlueprintCallable)
	EAbilityActivationType GetAbilityActivationType() const { return AbilityActivationType; }

	UFUNCTION(BlueprintCallable)
	bool GetIsActive() const { return IsActive; }
	
	UFUNCTION(BlueprintCallable)
	UTexture2D* GetAbilityDisplayImage() const { return AbilityDisplayImage; }
};
