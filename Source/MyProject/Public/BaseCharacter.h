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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	UStat* MovementSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
	TArray<FName> AttackBoneNames;
	
	UPROPERTY(BlueprintReadWrite, Category = Combat)
	bool IsAttacking;
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
	bool CanPunch = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = Combat)
	TArray<AActor*> CurrActorsHit;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat)
	float MainAttackTraceRadius = 50.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat)
	float MainAttackTraceDistance = 50.0f;
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = Combat)
	AActor* Target = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat)
	int InvulnerableCounter = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat)
	int RecoveringCounter = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat)
	bool IsRecovering = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat)
	bool IsParrying = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat)
	bool IsBlocking = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat)
	bool IsDodging = false; 
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat)
	bool IsInvulnerable = false; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CombatMontages )
	TArray<UAnimMontage*> CombatMontages;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CombatMontages )
	TArray<UAnimMontage*> ParryMontages;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<UAnimMontage*> HitReactionMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAbilitySystemComponent* AbilitySystem;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float Ammo;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Shooting)
	UAnimMontage* ShootMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Shooting)
	FVector2D AimOffsetRotation;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void MainAttack();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Block();
	
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void EndBlock();
	
	UFUNCTION(NetMulticast, Reliable)
	void PlayMainAttackMontage(int Index);

	UFUNCTION(NetMulticast, Reliable)
	void PlayParryMontage(int Index);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void PlayHitReactionMontage(int Index);

	bool CanAct();

	UFUNCTION(BlueprintCallable)
	bool HasTarget();
	
	void HandleHit();

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

	void OnCombatMontageEnd(UAnimMontage* Montage_, bool interrupted_);

	void OnParryMontageEnd(UAnimMontage* Montage_, bool interrupted_);

	void OnHitReactionMontageEnd(UAnimMontage* Montage_, bool interrupted_);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
