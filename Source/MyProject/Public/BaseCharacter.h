// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttackLevelEnum.h"
#include "GameFramework/Character.h"
#include "MotionWarpingComponent.h"
#include "BaseCharacter.generated.h"


enum class EAttackLevelEnum : uint8;
class UComboNode;
class UAbilitySystemComponent;
class UStat;
class UStatModifier;
class AProjectile;

USTRUCT(BlueprintType)
struct FDodgeMontageStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=DodgeMontages)
	UAnimMontage* NeutralDodge;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=DodgeMontages)
	UAnimMontage* ForwardDodge;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=DodgeMontages)
	UAnimMontage* BackDodge;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=DodgeMontages)
	UAnimMontage* RightDodge;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=DodgeMontages)
	UAnimMontage* LeftDodge;
};

USTRUCT(BlueprintType)
struct FHitMontageStruct
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=DodgeMontages)
	UAnimMontage* FrontHit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=DodgeMontages)
	UAnimMontage* RightHit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=DodgeMontages)
	UAnimMontage* LeftHit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=DodgeMontages)
	UAnimMontage* FrontHitH;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=DodgeMontages)
	UAnimMontage* RightHitH;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=DodgeMontages)
	UAnimMontage* LeftHitH;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=DodgeMontages)
	UAnimMontage* FrontHeadHit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=DodgeMontages)
	UAnimMontage* RightHeadHit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=DodgeMontages)
	UAnimMontage* LeftHeadHit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=DodgeMontages)
	UAnimMontage* FrontHeadHitH;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=DodgeMontages)
	UAnimMontage* RightHeadHitH;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=DodgeMontages)
	UAnimMontage* LeftHeadHitH;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=DodgeMontages)
	UAnimMontage* FrontLegHit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=DodgeMontages)
	UAnimMontage* RightLegHit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=DodgeMontages)
	UAnimMontage* LeftLegHit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=DodgeMontages)
	UAnimMontage* FrontLegHitH;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=DodgeMontages)
	UAnimMontage* RightLegHitH;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=DodgeMontages)
	UAnimMontage* LeftLegHitH;

};

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

	UPROPERTY(BlueprintReadWrite, Category = Combat)
	EAttackLevelEnum AttackLevel = EAttackLevelEnum::AE_LightAttack;
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
	bool CanPunch = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = Combat)
	TArray<AActor*> CurrActorsHit;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat)
	float MainAttackTraceRadius = 50.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat)
	float MainAttackTraceDistance = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat)
	float FistCollisionTraceRadius = 7.0f;
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
	ABaseCharacter* Target = nullptr;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
	FTransform TargetTransform;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat)
	int ParryCounter = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat)
	int MaxParryCounter = 30;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat)
	int PauseHitCounter = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat)
	int MaxPauseHitCounter = 10;
	
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
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool MainAttackIsCharging = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat)
	FVector LastFistCollisionLocation = FVector::Zero();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, Category = CombatMontages )
	TArray<UComboNode*> CombatMontages;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CombatMontages )
	int ComboCounter;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CombatMontages )
	UComboNode* CurrComboNode;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CombatMontages )
	TArray<UAnimMontage*> ParryMontages;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CombatMontages )
	FHitMontageStruct HitReactionMontages;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CombatMontages )
	FDodgeMontageStruct DodgeMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAbilitySystemComponent* AbilitySystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMotionWarpingComponent* MotionWarpingComponent;
	
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
	void SetHeavyAttack();
	
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void MainAttackRelease();
	
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Block();
	
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void EndBlock();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Dodge();
	
	UFUNCTION(NetMulticast, Reliable)
	void PlayMainAttackMontage(int Index);

	UFUNCTION(NetMulticast, Reliable)
	void PlayParryMontage(int Index);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void PlayHitReactionMontage(const FVector& Location, EAttackLevelEnum AttackLevelI);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void PlayDodgeMontage(FVector Direction);

	void UpdateLookAtTarget();

	void IncrementParryCounter(const int MaxCounterVal);
	
	bool CanAct();

	UFUNCTION(BlueprintCallable)
	bool HasTarget();
	
	void HandleHit(const FVector& HitLocation, EAttackLevelEnum AttackLevelI);

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

	TArray<FName> GetAttackBoneNames() const { return AttackBoneNames; }

	void OnCombatMontageEnd(UAnimMontage* Montage_, bool interrupted_);

	void OnParryMontageEnd(UAnimMontage* Montage_, bool interrupted_);

	void OnDodgeMontageEnd(UAnimMontage* Montage_, bool interrupted_);

	void OnHitReactionMontageEnd(UAnimMontage* Montage_, bool interrupted_);

	void SetLastFistCollision(FVector Location) { LastFistCollisionLocation = Location; }

	void SetFistCollisionTraceRadius(float TraceRadius) { FistCollisionTraceRadius = TraceRadius; }

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
