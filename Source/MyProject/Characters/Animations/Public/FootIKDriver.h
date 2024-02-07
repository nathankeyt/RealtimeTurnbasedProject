// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IKDriverBase.h"
#include "FootIKDriver.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, EditInlineNew)
class MYPROJECT_API UFootIKDriver : public UIKDriverBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	int LegsToMoveCount = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FootDistanceThreshold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* FootHorizontalCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* FootHeightCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FootStepHeight = 5.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FootTraceDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LegAlphaRate = 0.05;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FootTraceRadius = 5.0f;

public:
	UFUNCTION(BlueprintCallable)
	bool DoFootTrace(FIKBoneNode& BoneNode);
	
	void Update() override;

	UFUNCTION(BlueprintCallable)
	void SetBoneGroupShouldMove(FBoneGroup& BoneGroup);

	UFUNCTION(BlueprintCallable)
	void MoveLeg(FIKBoneNode& MovingLeg);

};