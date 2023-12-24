// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "TurnEnum.h"
#include "MainGameState.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API AMainGameState : public AGameState
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	ETurnEnum CurrentTurn;
	
	void SetCurrentTurn(ETurnEnum Turn) { CurrentTurn = Turn; GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Turn switch"))); }
	
};
