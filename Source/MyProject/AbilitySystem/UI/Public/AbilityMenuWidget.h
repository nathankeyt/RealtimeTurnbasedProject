// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AbilityMenuWidget.generated.h"

class UAbility;
class UAbilityEntryWidget;
class UTreeView;
class UListView;
/**
 * 
 */
UCLASS()
class MYPROJECT_API UAbilityMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UListView* AbilityListView;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//TSubclassOf<UAbilityEntryWidget> AbilityEntryClass;

public:
	void AddAbilityToList(UAbility* Ability) const;
};
