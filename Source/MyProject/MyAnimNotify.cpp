// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimNotify.h"
#include "PlayerCharacter.h"

void UMyAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) {
    APlayerCharacter* MyCharacter = Cast<APlayerCharacter>(MeshComp->GetOwner());
    if (MyCharacter != nullptr) {
        MyCharacter->SetPunchBoneName(BoneName);
        //comment
    }
}