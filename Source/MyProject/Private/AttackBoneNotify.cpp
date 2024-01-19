// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackBoneNotify.h"
#include "BaseCharacter.h"

void UAttackBoneNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) {
    ABaseCharacter* NotifiedCharacter = Cast<ABaseCharacter>(MeshComp->GetOwner());
    
    if (NotifiedCharacter != nullptr) {
        NotifiedCharacter->ClearAttackBoneNames();
        NotifiedCharacter->AddAttackBoneName(BoneName);
        NotifiedCharacter->SetFistCollisionTraceRadius(TraceRadius);
    }
}