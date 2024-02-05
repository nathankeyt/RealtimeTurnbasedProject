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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FBoneGroup> BoneGroups;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsMoving;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FootDistanceThreshold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* FootHorizontalCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* FootHeightCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FootTraceDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIKBoneNode MovingLeg;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LegAlphaRate = 0.05;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FootTraceRadius = 5.0f;

public:
	UFUNCTION(BlueprintCallable)
	FTransform DoFootTrace(FIKBoneNode BoneNode);

	UFUNCTION(BlueprintCallable)
	void UpdateFootPositions();

	UFUNCTION(BlueprintCallable)
	void SetLegShouldMove(FIKBoneNode BoneNode);

	UFUNCTION(BlueprintCallable)
	void MoveLegs();

	UFUNCTION(BlueprintCallable)
	void SetCharacter(ABaseCharacter* NewCharacter) { Character = NewCharacter; }

	UFUNCTION(BlueprintCallable)
	TArray<FName> GetBoneNames() { return BoneNames; }
	
	UFUNCTION(BlueprintCallable)
	TArray<FBoneGroup> GetBoneGroups() { return BoneGroups; }

};
