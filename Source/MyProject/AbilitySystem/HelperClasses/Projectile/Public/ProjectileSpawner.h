// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileSpawner.generated.h"

class ABaseCharacter;
class UStatModifierApplicator;
class UNiagaraSystem;
class AProjectile;
/**
 * 
 */
UCLASS(Blueprintable, EditInlineNew)
class MYPROJECT_API UProjectileSpawner : public UObject
{
	GENERATED_BODY()
	
public:
	UProjectileSpawner();

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY(BlueprintReadWrite, Category = Projectile)
	AProjectile* CurrProjectile;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	float ProjectileLifeSpan;

	UPROPERTY(EditAnywhere)
	UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere)
	UMaterial* Material;
	
	UPROPERTY(EditAnywhere)
	UNiagaraSystem* NiagaraSystem;

	UPROPERTY(EditAnywhere)
	UParticleSystem* ParticleSystem;

	UPROPERTY(EditAnywhere)
	UStatModifierApplicator* StatModifierApplicator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ABaseCharacter* ActiveCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector CurrMuzzleOffset;

	
public:
	UFUNCTION(BlueprintCallable)
	void SpawnProjectile(ABaseCharacter* Character);

	UFUNCTION(BlueprintCallable)
	void FireProjectile(FVector Velocity);

	UFUNCTION(BlueprintCallable)
	void FireProjectileAtLook(float Speed);

	UFUNCTION(BlueprintCallable)
	void SetMesh(UStaticMesh* Mesh_) { Mesh = Mesh_; }

	UFUNCTION(BlueprintCallable)
	void SetMaterial(UMaterial* Material_) { Material = Material_; }

	UFUNCTION(BlueprintCallable)
	void SetNiagaraSystem(UNiagaraSystem* NiagaraSystem_) { NiagaraSystem = NiagaraSystem_; }

	UFUNCTION(BlueprintCallable)
	void SetParticleSystem(UParticleSystem* ParticleSystem_) { ParticleSystem = ParticleSystem_; }

	UFUNCTION(BlueprintCallable)
	void SetStatModifierApplicator(UStatModifierApplicator* StatModifierApplicator_) { StatModifierApplicator = StatModifierApplicator_; }
};
