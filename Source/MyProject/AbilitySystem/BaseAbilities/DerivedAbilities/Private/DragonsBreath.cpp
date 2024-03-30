// Fill out your copyright notice in the Description page of Project Settings.


#include "MyProject/AbilitySystem/BaseAbilities/DerivedAbilities/Public/DragonsBreath.h"

#include "MyProject/AbilitySystem/HelperClasses/Projectile/Public/ProjectileSpawner.h"
#include "MyProject/Characters/Player/Public/PlayerCharacter.h"
#include "MyProject/Characters/Public/BaseCharacter.h"

bool UDragonsBreath::Activate(ABaseCharacter* Character)
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Character))
	{
		PlayerCharacter->SetAimOffset(true);

		if (PlayerCharacter->AnimationLayeringMap.Contains(EAnimationLayeringEnum::AL_PalmShoot)) {
			PlayerCharacter->AnimationLayeringMap[EAnimationLayeringEnum::AL_PalmShoot] = true;
		}	
	}

	ActiveCharacter = Character;
	IsActive = true;

	return true;
}

bool UDragonsBreath::EndActivation()
{
	if (ActiveCharacter != nullptr)
	{
		EndMainAction();
		
		if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(ActiveCharacter))
		{
			PlayerCharacter->SetAimOffset(false);

			if (PlayerCharacter->AnimationLayeringMap.Contains(EAnimationLayeringEnum::AL_PalmShoot)) {
				PlayerCharacter->AnimationLayeringMap[EAnimationLayeringEnum::AL_PalmShoot] = false;
			}	
		}

		ActiveCharacter = nullptr;
		IsActive = false;

		return true;
	}

	return false;
}

void UDragonsBreath::UseMainAction()
{
	if (ActiveCharacter != nullptr && ProjectileSpawner != nullptr)
	{
		IsMainActive = true;

		ActiveCharacter->GetWorldTimerManager().SetTimer(BreathHandle, this, &UDragonsBreath::WhileMainActive, 0.1f, true, 0.0f);
	}
}

void UDragonsBreath::WhileMainActive()
{
	if (ActiveCharacter != nullptr && ProjectileSpawner != nullptr)
	{
		ProjectileSpawner->SpawnProjectile(ActiveCharacter);

		ProjectileSpawner->FireProjectileAtLook(30000.0f);
	}
}



void UDragonsBreath::EndMainAction()
{
	if (ActiveCharacter != nullptr && ProjectileSpawner != nullptr)
	{
		ActiveCharacter->GetWorldTimerManager().ClearTimer(BreathHandle);
	}
}
