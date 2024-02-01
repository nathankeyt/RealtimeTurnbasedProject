// Fill out your copyright notice in the Description page of Project Settings.


#include "EnergyShot.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "PlayerCharacter.h"
#include "ProjectileSpawner.h"

bool UEnergyShot::Activate(ABaseCharacter* Character)
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Character))
	{
		PlayerCharacter->SetAimOffset(true);

		ActiveCharacter = Character;
		
		return true;
	}

	return false;
}

bool UEnergyShot::EndActivation()
{
	if (ActiveCharacter != nullptr)
	{
		if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(ActiveCharacter))
		{
			if (ActiveChargeEffect != nullptr)
			{
				ActiveChargeEffect->Deactivate();
			}
		
			PlayerCharacter->SetAimOffset(false);
			
			ActiveCharacter = nullptr;

			return true;
		}
	}

	return false;
}

void UEnergyShot::Charge()
{
	if (ActiveCharacter != nullptr)
	{
		FTransform BoneTransform = ActiveCharacter->GetMesh()->GetBoneTransform("index_03_l");

		ActiveChargeEffect = UNiagaraFunctionLibrary::SpawnSystemAttached(
			ChargeEffectTemplate,
			ActiveCharacter->GetMesh(),
			"index_03_l",
			BoneTransform.GetLocation(),
			BoneTransform.Rotator(),
			EAttachLocation::KeepWorldPosition,
			true);
	}
}


void UEnergyShot::Fire()
{
	if (ActiveCharacter != nullptr && ProjectileSpawner != nullptr)
	{
		if (ActiveChargeEffect != nullptr)
		{
			ActiveChargeEffect->Deactivate();
		}
		
		ProjectileSpawner->SpawnProjectile(ActiveCharacter);

		ProjectileSpawner->FireProjectileAtLook(30000.0f);
	}
}
