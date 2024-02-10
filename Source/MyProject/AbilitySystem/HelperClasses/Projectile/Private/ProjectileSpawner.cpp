// Fill out your copyright notice in the Description page of Project Settings.


#include "MyProject/AbilitySystem/HelperClasses/Projectile/Public/ProjectileSpawner.h"

#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "MyProject/AbilitySystem/HelperClasses/Projectile/Public/Projectile.h"
#include "MyProject/Characters/Player/Public/PlayerCharacter.h"


UProjectileSpawner::UProjectileSpawner()
{
    ProjectileClass = AProjectile::StaticClass();
}


void UProjectileSpawner::SpawnProjectile(ABaseCharacter* Character)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Character);
    
    if (ProjectileClass && PlayerCharacter != nullptr) {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("after class check")));
        FVector CharacterLocation = Character->GetMesh()->GetBoneLocation("index_03_l");

        FVector MuzzleOffset;
        MuzzleOffset.Set(100.0f, 0.0f, 0.0f);

        UCameraComponent* FollowCamera = PlayerCharacter->GetFollowCamera();

        FVector MuzzleLocation = CharacterLocation + FTransform(FollowCamera->GetComponentRotation()).TransformVector(MuzzleOffset); // try muzzleroation.vector() * muzzleoffset
        
        if (UWorld* World = PlayerCharacter->GetWorld()) {
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("world")));

            const FTransform SpawnTransform(FollowCamera->GetComponentRotation(), MuzzleLocation);
            
            if (AProjectile* Projectile = World->SpawnActorDeferred<AProjectile>(ProjectileClass, SpawnTransform, PlayerCharacter, PlayerCharacter->GetInstigator())) {
                Projectile->SetupSpawn(Weapon, Mesh, Material, NiagaraSystem, ParticleSystem);

                Projectile->FinishSpawning(SpawnTransform);
                
                GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("projectile")));
                PlayerCharacter->PlayAnimMontage(PlayerCharacter->ShootMontage);
                
                CurrProjectile = Projectile;
                /*
                if (PlayerHUD) {
                    PlayerHUD->SetAmmo(--Ammo);
                }	*/
                ActiveCharacter = PlayerCharacter;
            }
        }
    }
}

void UProjectileSpawner::FireProjectile(FVector Velocity)
{
    CurrProjectile->Fire(Velocity, ProjectileLifeSpan);
    ActiveCharacter = nullptr;
}

void UProjectileSpawner::FireProjectileAtLook(float Speed)
{
    if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(ActiveCharacter))
    {
        FVector BoneLocation = ActiveCharacter->GetMesh()->GetBoneLocation("index_03_l");
        UCameraComponent* FollowCamera = PlayerCharacter->GetFollowCamera();

        FVector MuzzleLocation = BoneLocation; // FTransform(FollowCamera->GetComponentRotation()).TransformVector(FVector::Zero()); // try muzzleroation.vector() * muzzleoffset
        
        FHitResult HitResult;

        FRotator MuzzleRotation;

        float Distance = 5000.0f;

        if (DoFollowCameraTrace(HitResult, PlayerCharacter, Distance))
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("projectile trace hit")));
            MuzzleRotation = UKismetMathLibrary::FindLookAtRotation(MuzzleLocation, HitResult.Location);
        }
        else
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("projectile trace not hit")));
            MuzzleRotation = UKismetMathLibrary::FindLookAtRotation(MuzzleLocation, MuzzleLocation + FollowCamera->GetForwardVector() * 9000.0f);
        }

        const FVector LaunchDirection = MuzzleRotation.Vector();
        
        CurrProjectile->Fire(LaunchDirection * Speed, ProjectileLifeSpan);
    }
    
    ActiveCharacter = nullptr;
}
