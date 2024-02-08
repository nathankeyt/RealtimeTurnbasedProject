// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.generated.h"

class UWeapon;
class USphereComponent;
class UNiagaraSystem;
class UNiagaraComponent;
class UProjectileMovementComponent;


class UStatModifierApplicator;

UCLASS()
class MYPROJECT_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* CollisionComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	UMaterialInstanceDynamic* ProjectileMaterialInstance;

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	UStaticMeshComponent* ProjectileMeshComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	UNiagaraComponent* ProjectileParticleSystemComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	UParticleSystem* ProjectileCollisionParticleSystem;

	UPROPERTY(VisibleAnywhere, Category = Movement)
	UProjectileMovementComponent* ProjectileMovementComponent;
	
	UPROPERTY(VisibleAnywhere, Category = Stats)
	UWeapon* Weapon;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void SetupSpawn(UWeapon* NewWeapon = nullptr,
		UStaticMesh* Mesh = nullptr,
		UMaterial* Material = nullptr,
		UNiagaraSystem* NiagaraSystem  = nullptr,
		UParticleSystem* ParticleSystem  = nullptr);
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Fire(FVector Velocity, float ProjectileLifeSpan = 3.0f);
};

