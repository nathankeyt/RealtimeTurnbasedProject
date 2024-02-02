// Fill out your copyright notice in the Description page of Project Settings.


#include "MyProject/AbilitySystem/BaseAbilities/DerivedAbilities/Public/EnergyShot.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "MyProject/AbilitySystem/HelperClasses/Projectile/Public/ProjectileSpawner.h"
#include "MyProject/Characters/Player/Public/PlayerCharacter.h"
#include "MyProject/Characters/Public/BaseCharacter.h"

bool UEnergyShot::Activate(ABaseCharacter* Character)
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Character))
	{
		PlayerCharacter->SetAimOffset(true);

		ActiveCharacter = Character;
		IsActive = true;
		
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
			IsActive = false;

			return true;
		}
	}

	return false;
}

void UEnergyShot::UseMainAction()
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


void UEnergyShot::EndMainAction()
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


void UEnergyShot::UseAltAction()
{
	
}

void UEnergyShot::EndAltAction()
{
	
}