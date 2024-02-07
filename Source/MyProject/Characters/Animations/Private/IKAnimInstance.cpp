// Fill out your copyright notice in the Description page of Project Settings.


#include "MyProject/Characters/Animations/Public/IKAnimInstance.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MyProject/Characters/Public/BaseCharacter.h"

void UIKAnimInstance::UpdateFootPositions()
{
	for (FBoneGroup& BoneGroup : BoneGroups)
	{
		for (int i = 0; i < BoneGroup.Num(); i++)
		{
			FIKBoneNode& BoneNode = BoneGroup.BoneGroup[i];
			
			if (DoFootTrace(BoneNode)) {
				SetBoneGroupShouldMove(BoneGroup);
			}

			if (BoneNode.ShouldMove)
			{
				MoveLeg(BoneNode);
			}
		}
	}
}

void UIKAnimInstance::SetBoneGroupShouldMove(FBoneGroup& BoneGroup)
{
	if (!LegsToMoveCount)
	{
		LegsToMoveCount = BoneGroup.Num();

		for (int i = 0; i < BoneGroup.Num(); i++)
		{
			FIKBoneNode& BoneNode = BoneGroup.BoneGroup[i];
			BoneNode.ShouldMove = true;
			BoneNode.PrevTransform = BoneNode.ControlTransform;
		}
	}
}

void UIKAnimInstance::MoveLeg(FIKBoneNode& MovingLeg)
{
	if (Character != nullptr && FootHorizontalCurve != nullptr && FootHeightCurve != nullptr)
	{
		MovingLeg.LegAlpha += LegAlphaRate;
		MovingLeg.ControlTransform.SetTranslation(UKismetMathLibrary::TLerp(MovingLeg.PrevTransform, MovingLeg.TargetTransform, FootHorizontalCurve->GetFloatValue(MovingLeg.LegAlpha)).GetTranslation() + (FVector::ZAxisVector * FootStepHeight * FootHeightCurve->GetFloatValue(MovingLeg.LegAlpha)));
		
		if (MovingLeg.LegAlpha >= 1.0f)
		{
			MovingLeg.ControlTransform = MovingLeg.TargetTransform;
			MovingLeg.LegAlpha = 0.0f;
			LegsToMoveCount -= 1;
			MovingLeg.ShouldMove = false;
		}
	}
}


bool UIKAnimInstance::DoFootTrace(FIKBoneNode& BoneNode)
{
	if (Character != nullptr)
	{
		TArray<AActor*> IgnoreActors;

		IgnoreActors.Add(Character);

		FHitResult HitResult;

		const FVector BoneLocation = Character->GetMesh()->GetBoneLocation(BoneNode.BoneName);

		
		
		const bool Hit = UKismetSystemLibrary::SphereTraceSingle(
			Character->GetWorld(),
			BoneLocation,
			BoneLocation + (Character->GetVelocity() * 0.25f) - (FVector::ZAxisVector * FootTraceDistance),
			FootTraceRadius,
			TraceTypeQuery1,
			false,
			IgnoreActors,
			EDrawDebugTrace::ForDuration,
			HitResult,
			true);

		const FVector HitLocationDiff = HitResult.Location - BoneNode.ControlTransform.GetTranslation();

		if (Hit && HitLocationDiff.Length() > FootDistanceThreshold)
		{
			BoneNode.TargetTransform = FTransform(FRotator::ZeroRotator, HitResult.Location, FVector(FootTraceRadius));

			return true;
		}
	}
	
	return false;
}




