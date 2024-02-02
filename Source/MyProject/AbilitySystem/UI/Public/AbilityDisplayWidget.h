// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AbilityDisplayWidget.generated.h"

class UAbilitySystemComponent;
class UImage;
/**
 * 
 */
UCLASS()
class MYPROJECT_API UAbilityDisplayWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UImage*> AbilityDisplays;

	UFUNCTION(BlueprintCallable)
	TArray<UImage*> GetAbilityDisplays() const {return AbilityDisplays; }

	UFUNCTION(BlueprintCallable)
	void SetAbilityDisplay(UAbilitySystemComponent* AbilitySystem);
};
