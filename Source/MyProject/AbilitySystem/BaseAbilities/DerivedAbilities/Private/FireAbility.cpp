// Fill out your copyright notice in the Description page of Project Settings.


#include "MyProject/AbilitySystem/BaseAbilities/DerivedAbilities/Public/FireAbility.h"

#include "MyProject/AbilitySystem/HelperClasses/Projectile/Public/ProjectileSpawner.h"

bool UFireAbility::Activate(ABaseCharacter* Character)
{
	if (Character != nullptr && ProjectileSpawner != nullptr)
	{
		ProjectileSpawner->SpawnProjectile(Character);

		return true;
	}

	return false;
}

bool UFireAbility::EndActivation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("ability deactivation called")));

	if (ProjectileSpawner != nullptr)
	{
		ProjectileSpawner->FireProjectile(FVector::Zero());

		return true;
	}

	return false;
}
