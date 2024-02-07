// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IKAnimInstance.generated.h"


class UIKDriverBase;

UCLASS()
class MYPROJECT_API UIKAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	TArray<UIKDriverBase*> IKDrivers;
	
};
