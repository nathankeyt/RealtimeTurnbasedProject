// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatWidget.generated.h"

class UProgressBar;
/**
 * 
 */
UCLASS()
class MYPROJECT_API UStatWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UProgressBar* StaminaBar;

public:
	void DisplayHealthPercentage(const float CurrHealth, const float MaxHealth) const;

	void DisplayStaminaPercentage(const float CurrStamina, const float MaxStamina) const;
	
};
