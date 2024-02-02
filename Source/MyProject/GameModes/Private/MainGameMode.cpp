// Fill out your copyright notice in the Description page of Project Settings.


#include "MyProject/GameModes/Public/MainGameMode.h"

#include "MyProject/GameStates/Public/MainGameState.h"
#include "UObject/ConstructorHelpers.h"

AMainGameMode::AMainGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	
}

void AMainGameMode::SwitchTurn(ETurnEnum Turn)
{
	GetGameState<AMainGameState>()->SetCurrentTurn(Turn);
}

