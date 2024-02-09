// Fill out your copyright notice in the Description page of Project Settings.



#include "MyProject/Combat/Notifies/Public/AttackBoneNotify.h"

#include "MyProject/Characters/Public/BaseCharacter.h"


void UAttackBoneNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) {
    ABaseCharacter* NotifiedCharacter = Cast<ABaseCharacter>(MeshComp->GetOwner());
    
    if (NotifiedCharacter != nullptr) {
        NotifiedCharacter->SetHeavyAttack();
        NotifiedCharacter->ClearAttackBoneNames();
        NotifiedCharacter->AddAttackBoneName(BoneName);
        NotifiedCharacter->SetFistCollisionTraceRadius(TraceRadius);
        NotifiedCharacter->AttackRelease();
    }
}
