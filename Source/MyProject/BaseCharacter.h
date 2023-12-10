// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Stat.h"
#include "StatModifier.h"
#include "BaseCharacter.generated.h"


class UAbilitySystemComponent;

UCLASS(Abstract)
class MYPROJECT_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced,  meta = (AllowPrivateAccess = "true"))
	UStat* Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced,  meta = (AllowPrivateAccess = "true"))
	UStat* Mana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced,  meta = (AllowPrivateAccess = "true"))
	UStat* MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly,  meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> CombatMontages;

	UPROPERTY(EditAnywhere, BlueprintReadOnly,  meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> HitReactionMontages;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FName PunchBoneName = FName(TEXT("hand_l"));
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool IsPunching = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  meta = (AllowPrivateAccess = "true"))
	TArray<AActor*> CurrPunchHits;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,  meta = (AllowPrivateAccess = "true"))
	UAbilitySystemComponent* AbilitySystem;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetPunchBoneName(FName PunchBoneName_) { PunchBoneName = PunchBoneName_; }
	
	void SetMovementSpeed(UStat* Stat);

	void UpdateMovementSpeed();

	void AddMovementSpeedModifier(UStatModifier* StatModifier);
	
	UStat* GetMovementSpeed() const { return MovementSpeed; };

	void OnPunchingMontageEnd(UAnimMontage* Montage_, bool interrupted_);

	void PlayHitReaction();

};
