// Fill out your copyright notice in the Description page of Project Settings.


#include "MyProject/AbilitySystem/BaseAbilities/DerivedAbilities/Public/BasicProjectile.h"

#include "MyProject/AbilitySystem/HelperClasses/Projectile/Public/ProjectileSpawner.h"
#include "MyProject/Characters/Public/BaseCharacter.h"

UBasicProjectile::UBasicProjectile()
{
	// ProjectileSpawner = CreateDefaultSubobject<UProjectileSpawner>(TEXT("ProjectileSpawner"));
	// ProjectileSpawner = NewObject<UProjectileSpawner>();
	
}


bool UBasicProjectile::Activate(ABaseCharacter* Character)
{
    if (Character != nullptr  && ProjectileSpawner != nullptr)
    {
    	if (StatModifierApplicator != nullptr)
    	{
    		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("statmodifierapplied")));
    		ProjectileSpawner->SetStatModifierApplicator(StatModifierApplicator);
    	}

	    ProjectileSpawner->SpawnProjectile(Character);
    	ProjectileSpawner->FireProjectile(Character->GetActorForwardVector() * 30000.0f);

		return true;
    }

	return false;
}

