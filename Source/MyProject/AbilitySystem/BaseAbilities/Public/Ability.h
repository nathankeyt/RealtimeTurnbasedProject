// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Ability.generated.h"

class UStat;
enum class EStatEnum : uint8;
class UAbilityModifierContainer;
class UAbilityModifier;
enum class EAbilityComponentType : uint8;
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* AbilityDisplayImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	UStatModifierApplicator* StatModifierApplicator;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ManaCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ActivationSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAbilityType AbilityType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAbilityActivationType AbilityActivationType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ABaseCharacter* ActiveCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsActive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	TMap<EStatEnum, UStat*> StatMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EAbilityComponentType, UAbilityModifierContainer*> AbilityModifierMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText DisplayText;

public:
	UFUNCTION(BlueprintCallable)
	virtual bool Activate(ABaseCharacter* Character) { return false; };

	UFUNCTION(BlueprintCallable)
	virtual bool EndActivation() { return false; };

	virtual void UseMainAction() {};

	virtual void EndMainAction() {};

	virtual void UseAltAction() {};

	virtual void EndAltAction() {};

	virtual void WhileActive() {};

	virtual void OnHit() {};

	UFUNCTION(BlueprintCallable)
	EAbilityType GetAbilityType() const { return AbilityType; }

	UFUNCTION(BlueprintCallable)
	EAbilityActivationType GetAbilityActivationType() const { return AbilityActivationType; }

	UFUNCTION(BlueprintCallable)
	bool GetIsActive() const { return IsActive; }
	
	UFUNCTION(BlueprintCallable)
	UTexture2D* GetAbilityDisplayImage() const { return AbilityDisplayImage; }
};
