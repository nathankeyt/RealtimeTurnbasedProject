// Fill out your copyright notice in the Description page of Project Settings.


#include "ResetComboNotify.h"
#include "BaseCharacter.h"

void UResetComboNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	ABaseCharacter* NotifiedCharacter = Cast<ABaseCharacter>(MeshComp->GetOwner());
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("attempting to cast to base character")));
	if (NotifiedCharacter != nullptr) {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("attempting to set can punch to true")));
		NotifiedCharacter->SetCanPunch(true);
	}
}
