// Fill out your copyright notice in the Description page of Project Settings.


#include "EnergyShot.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "PlayerCharacter.h"
#include "ProjectileSpawner.h"

void UEnergyShot::Activate(ABaseCharacter* Character)
{
	ActiveCharacter = Character;
	
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("after class check")));

	FTransform BoneTransform = ActiveCharacter->GetMesh()->GetBoneTransform("index_03_l");

	ActiveChargeEffect = UNiagaraFunctionLibrary::SpawnSystemAttached(ChargeEffectTemplate, Character->GetMesh(), "index_03_l", BoneTransform.GetLocation(), BoneTransform.Rotator(), EAttachLocation::KeepWorldPosition, true);
}

void UEnergyShot::EndActivation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("ability deactivation called")));
	if (ActiveCharacter != nullptr && ProjectileSpawner != nullptr)
	{
		ProjectileSpawner->SpawnProjectile(ActiveCharacter);

		ProjectileSpawner->FireProjectileAtLook(30000.0f);

		ActiveCharacter = nullptr;
	}

	if (ActiveChargeEffect != nullptr)
	{
		ActiveChargeEffect->Deactivate();
	}
}