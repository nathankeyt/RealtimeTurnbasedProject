// Fill out your copyright notice in the Description page of Project Settings.


#include "Myproject/AbilitySystem/HelperClasses/Trace/Public/ThirdPersonTraceDriver.h"

#include "Camera/CameraComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MyProject/Characters/Player/Public/PlayerCharacter.h"

// Add default functionality here for any IThirdPersonTraceDriver functions that are not pure virtual.

bool IThirdPersonTraceDriver::DoFollowCameraTrace(FHitResult& HitResult, APlayerCharacter* Character, float Distance)
{
	if (Character != nullptr) {
		const UCameraComponent* FollowCamera = Character->GetFollowCamera();

		const FVector Start = FollowCamera->GetComponentLocation();
		const FVector End = Start + (FollowCamera->GetForwardVector() * Distance);

		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(Character);

		/*
		const bool Hit = GetWorld()->LineTraceSingleByChannel(
			HitResult,
			Start,
			End,
			ECollisionChannel::ECC_Visibility,
			QueryParams);
		*/

		return UKismetSystemLibrary::LineTraceSingle(
			Character->GetWorld(),
			Start,
			End,
			UEngineTypes::ConvertToTraceType(ECC_Pawn),
			false,
			ActorsToIgnore,
			EDrawDebugTrace::None,
			HitResult,
			true);
	}

	return false;
}
