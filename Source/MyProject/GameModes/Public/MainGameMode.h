// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MainGameMode.generated.h"

enum class ETurnEnum : uint8;
/**
 * 
 */
UCLASS()
class MYPROJECT_API AMainGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AMainGameMode();

	UFUNCTION(BlueprintCallable)
	void SwitchTurn(ETurnEnum Turn);
};
