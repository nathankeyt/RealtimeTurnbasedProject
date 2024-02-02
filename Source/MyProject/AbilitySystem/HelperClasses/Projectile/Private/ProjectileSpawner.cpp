// Fill out your copyright notice in the Description page of Project Settings.


#include "MyProject/AbilitySystem/HelperClasses/Projectile/Public/ProjectileSpawner.h"

#include "Kismet/KismetMathLibrary.h"

#include "Camera/CameraComponent.h"
#include "Kismet/KismetSystemLibrary.h"
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
                Projectile->SetupSpawn(StatModifierApplicator, Mesh, Material, NiagaraSystem, ParticleSystem);

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
        UCameraComponent* FollowCamera = PlayerCharacter->GetFollowCamera();
        FVector CharacterLocation = ActiveCharacter->GetMesh()->GetBoneLocation("index_03_l");

        FVector MuzzleLocation = CharacterLocation + FTransform(FollowCamera->GetComponentRotation()).TransformVector(CurrMuzzleOffset); // try muzzleroation.vector() * muzzleoffset

        const FVector Start = FollowCamera->GetComponentLocation();
        const FVector End = Start + (FollowCamera->GetForwardVector() * 9000.0f);

        TArray<AActor*> ActorsToIgnore;
        ActorsToIgnore.Add(PlayerCharacter);

        FHitResult HitResult;

        /*
        const bool Hit = GetWorld()->LineTraceSingleByChannel(
            HitResult,
            Start,
            End,
            ECollisionChannel::ECC_Visibility,
            QueryParams);
        */

        const bool Hit = UKismetSystemLibrary::LineTraceSingle(
            PlayerCharacter->GetWorld(),
            Start,
            End,
            UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility),
            false,
            ActorsToIgnore,
            EDrawDebugTrace::None,
            HitResult,
            true);

        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("after get world")));

        FRotator MuzzleRotation;

        if (Hit) {
            MuzzleRotation = UKismetMathLibrary::FindLookAtRotation(MuzzleLocation, HitResult.Location);
        }
        else {
            MuzzleRotation = UKismetMathLibrary::FindLookAtRotation(MuzzleLocation, End);
        }

        FVector LaunchDirection = MuzzleRotation.Vector();
    
        CurrProjectile->Fire(LaunchDirection * Speed, ProjectileLifeSpan);
    }
    ActiveCharacter = nullptr;
}
