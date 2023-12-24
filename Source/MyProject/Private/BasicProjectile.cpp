// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicProjectile.h"
#include "ProjectileSpawner.h"

UBasicProjectile::UBasicProjectile()
{
	// ProjectileSpawner = CreateDefaultSubobject<UProjectileSpawner>(TEXT("ProjectileSpawner"));
	// ProjectileSpawner = NewObject<UProjectileSpawner>();
	
}


void UBasicProjectile::Activate(ABaseCharacter* Character)
{
    if (ProjectileSpawner != nullptr)
    {
    	if (StatModifierApplicator != nullptr)
    	{
    		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("statmodifierapplied")));
    		ProjectileSpawner->SetStatModifierApplicator(StatModifierApplicator);
    	}
    	
	    ProjectileSpawner->ShootProjectile(Character);
    }
}

