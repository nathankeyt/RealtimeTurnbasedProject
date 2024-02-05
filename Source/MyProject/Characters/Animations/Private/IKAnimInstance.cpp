// Fill out your copyright notice in the Description page of Project Settings.


#include "MyProject/Characters/Animations/Public/IKAnimInstance.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Math/TransformCalculus3D.h"
#include "MyProject/Characters/Public/BaseCharacter.h"

void UIKAnimInstance::UpdateFootPositions()
{
	for (FBoneGroup BoneGroup : BoneGroups)
	{
		for (int i = 0; i < BoneGroup.BoneGroup.Num(); i++)
		{
			FIKBoneNode BoneNode = BoneGroup.BoneGroup[i];
			
			FTransform NewTransform = DoFootTrace(BoneNode);
			
			if (!NewTransform.TranslationEquals(BoneNode.TargetTransform)) {
				BoneNode.TargetTransform = NewTransform;

				SetLegShouldMove(BoneNode);
			}
		}
	}
}

void UIKAnimInstance::SetLegShouldMove(FIKBoneNode BoneNode)
{
	IsMoving = true;
	BoneNode.PrevTransform = BoneNode.ControlTransform;
	MovingLeg = BoneNode;
}

void UIKAnimInstance::MoveLegs()
{
	if (IsMoving && Character != nullptr && FootHorizontalCurve != nullptr && FootHeightCurve != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("leg alpha: %f"), MovingLeg.LegAlpha));
		MovingLeg.LegAlpha += LegAlphaRate;
		MovingLeg.ControlTransform = UKismetMathLibrary::TLerp(MovingLeg.PrevTransform, MovingLeg.TargetTransform, FootHorizontalCurve->GetFloatValue(MovingLeg.LegAlpha));
		
		if (MovingLeg.LegAlpha >= 1.0f)
		{
			MovingLeg.LegAlpha = 0.0f;
			IsMoving = false;
		}
	}
}



FTransform UIKAnimInstance::DoFootTrace(FIKBoneNode BoneNode)
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
			BoneLocation - (FVector::ZAxisVector * FootTraceDistance),
			FootTraceRadius,
			TraceTypeQuery1,
			false,
			IgnoreActors,
			EDrawDebugTrace::ForDuration,
			HitResult,
			true);

		const FVector HitLocationDiff = HitResult.Location - BoneLocation;

		if (Hit && HitLocationDiff.Length() > FootDistanceThreshold)
		{
			return FTransform(FRotator::ZeroRotator, HitResult.Location, FVector(FootTraceRadius));
		}
	}
	
	return BoneNode.TargetTransform;
}




