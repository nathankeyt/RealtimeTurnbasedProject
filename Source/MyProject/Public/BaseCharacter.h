// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"


class UAbilitySystemComponent;
class UStat;
class UStatModifier;
class AProjectile;

UCLASS(Abstract)
class MYPROJECT_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	UStat* Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	UStat* Stamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	UStat* BlockPercentage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	UStat* Mana;

	UPROPERTY(BlueprintReadWrite, Instanced)
	UStat* MovementSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<FName> AttackBoneNames;
	
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool IsAttacking;
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool CanPunch = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  meta = (AllowPrivateAccess = "true"))
	TArray<AActor*> CurrActorsHit;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trace", meta = (AllowPrivateAccess = "true"))
	float MainAttackTraceRadius = 50.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trace", meta = (AllowPrivateAccess = "true"))
	float MainAttackTraceDistance = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<UAnimMontage*> CombatMontages;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<UAnimMontage*> HitReactionMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAbilitySystemComponent* AbilitySystem;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float Ammo;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	UAnimMontage* ShootMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	FVector2D AimOffsetRotation;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = "Trace", meta = (AllowPrivateAccess = "true"))
	AActor* Target = nullptr;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UFUNCTION(Server, Reliable)
	void MainAttack();
	
	UFUNCTION(NetMulticast, Reliable)
	void PlayMainAttackMontage();

	UFUNCTION(NetMulticast, Reliable)
	void PlayHitReactionMontage();

	void ActivateAbility(const int AbilityIndex);

	void CheckFistCollision(FName BoneName);
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void AddAttackBoneName(FName BoneName) { AttackBoneNames.Add(BoneName); }

	void ClearAttackBoneNames() { AttackBoneNames.Empty(); }

	void SetCanPunch(bool CanPunch_ ) { CanPunch = CanPunch_; }
	
	void SetMovementSpeed(UStat* Stat);

	void UpdateMovementSpeed();

	void AddMovementSpeedModifier(UStatModifier* StatModifier);
	
	UStat* GetMovementSpeed() const { return MovementSpeed; }

	void OnPunchingMontageEnd(UAnimMontage* Montage_, bool interrupted_);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
