// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Stat.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, EditInlineNew)
class MYPROJECT_API UStat : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float Data = 1.0f;

public:
	UFUNCTION(BlueprintCallable)
	virtual float GetData() const { return Data; }

	UFUNCTION(BlueprintCallable)
	void SetData(const float Data_) { Data = Data_; }

	UFUNCTION(BlueprintCallable)
	void AddData(const float Data_) { Data += Data_; }
};
