// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "IKAnimInstance.generated.h"

class ABaseCharacter;

USTRUCT(BlueprintType)
struct FIKBoneNode
{
	GENERATED_BODY()

public:
	FIKBoneNode() = default;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName BoneName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform TargetTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform ControlTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform PrevTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LegAlpha = 0.0f;

	UPROPERTY(BlueprintReadWrite)
	bool ShouldMove = false;
};

USTRUCT(BlueprintType)
struct FBoneGroup{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FIKBoneNode> BoneGroup;
	
	FIKBoneNode operator[] (int32 i) {
		return BoneGroup[i];
	}

	void Add(FIKBoneNode Bone) {
		BoneGroup.Add(Bone);
	}

	int Num() {
		return BoneGroup.Num();
	}

	void Empty() {
		BoneGroup.Empty();
	}
};
/**
 * 
 */
UCLASS()
class MYPROJECT_API UIKAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ABaseCharacter* Character;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> BoneNames;

	UPROPERTY(BlueprintReadWrite)
	int LegsToMoveCount = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FBoneGroup> BoneGroups;
	
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

	UFUNCTION(BlueprintCallable)
	void UpdateFootPositions();

	UFUNCTION(BlueprintCallable)
	void SetBoneGroupShouldMove(FBoneGroup& BoneGroup);

	UFUNCTION(BlueprintCallable)
	void MoveLeg(FIKBoneNode& MovingLeg);

	UFUNCTION(BlueprintCallable)
	void SetCharacter(ABaseCharacter* NewCharacter) { Character = NewCharacter; }

	UFUNCTION(BlueprintCallable)
	TArray<FName> GetBoneNames() { return BoneNames; }
	
	UFUNCTION(BlueprintCallable)
	TArray<FBoneGroup> GetBoneGroups() { return BoneGroups; }

};
