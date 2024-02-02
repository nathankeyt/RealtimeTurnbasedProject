// Fill out your copyright notice in the Description page of Project Settings.


#include "MyProject/AbilitySystem/BaseAbilities/DerivedAbilities/Public/FlamingFists.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "MyProject/Characters/Public/BaseCharacter.h"

bool UFlamingFists::Activate(ABaseCharacter* Character)
{
	if (Character != nullptr)
	{
		const FTransform BoneTransformL = Character->GetMesh()->GetBoneTransform("middle_01_l");
		const FTransform BoneTransformR = Character->GetMesh()->GetBoneTransform("middle_01_r");
		
		ActiveNiagaraEffects.Add(UNiagaraFunctionLibrary::SpawnSystemAttached(
			NiagaraEffectTemplate,
			Character->GetMesh(),
			"middle_01_l",
			BoneTransformL.GetLocation(),
			BoneTransformL.Rotator(),
			EAttachLocation::KeepWorldPosition,
			true));

		ActiveNiagaraEffects.Add(UNiagaraFunctionLibrary::SpawnSystemAttached(
			NiagaraEffectTemplate,
			Character->GetMesh(),
			"middle_01_r",
			BoneTransformR.GetLocation(),
			BoneTransformL.Rotator(),
			EAttachLocation::KeepWorldPosition,
			true));

		ActiveCharacter = Character;
		IsActive = true;

		return true;
	}

	return false;
}

bool UFlamingFists::EndActivation()
{
	if (ActiveCharacter != nullptr)
	{
		for (UNiagaraComponent* NiagaraComponent : ActiveNiagaraEffects)
		{
			NiagaraComponent->Deactivate();
		}

		ActiveNiagaraEffects.Empty();
		
		ActiveCharacter = nullptr;
		IsActive = false;
		return true;
	}

	return false;
}

