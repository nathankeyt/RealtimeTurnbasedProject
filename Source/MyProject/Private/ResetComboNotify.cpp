// Fill out your copyright notice in the Description page of Project Settings.


#include "ResetComboNotify.h"
#include "BaseCharacter.h"

void UResetComboNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	ABaseCharacter* NotifiedCharacter = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (NotifiedCharacter != nullptr) {
		NotifiedCharacter->SetCanPunch(true);
	}
}
