// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimationLayeringEnum.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EAnimationLayeringEnum : uint8
{
	AL_PalmShoot	UMETA(DisplayName = "PalmShoot"),
	AL_FingerShoot	UMETA(DisplayName = "FingerShoot")
};
