#pragma once

#include "CoreMinimal.h"
#include "IKDriverBase.generated.h"

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

UCLASS(Abstract)
class MYPROJECT_API UIKDriverBase : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ABaseCharacter* Character;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FBoneGroup> BoneGroups;
	
public:
	UFUNCTION(BlueprintCallable)
	virtual void Update() {};

	UFUNCTION(BlueprintCallable)
	void SetCharacter(ABaseCharacter* NewCharacter) { Character = NewCharacter; }

	UFUNCTION(BlueprintCallable)
	TArray<FBoneGroup> GetBoneGroups() { return BoneGroups; }
};
