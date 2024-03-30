// Fill out your copyright notice in the Description page of Project Settings.


#include "MyProject/Characters/Public/BaseCharacter.h"

#include "NiagaraFunctionLibrary.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MyProject/AbilitySystem/BaseAbilities/Public/Ability.h"
#include "MyProject/AbilitySystem/Public/AbilitySystemComponent.h"
#include "MyProject/Combat/Enums/Public/KnockbackEnum.h"
#include "MyProject/Combat/Public/EquippedWeapon.h"
#include "MyProject/Combat/Public/Weapon.h"
#include "MyProject/Stats/Public/Stat.h"
#include "MyProject/Stats/StatModifiers/Public/StatModifier.h"
#include "MyProject/Stats/UI/Public/StatWidget.h"
#include "Net/UnrealNetwork.h"
#include "MyProject/Stats/Enums/Public/StatEnum.h"


// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	AbilitySystem = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem"));
	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	UpdateHealthUI();

	UpdateStaminaUI();

	UpdateMovementSpeed();
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UKismetSystemLibrary::FlushPersistentDebugLines(GetWorld());

	IncrementParryCounter(MaxParryCounter);
	
	AimRotator = UKismetMathLibrary::NormalizedDeltaRotator(GetControlRotation(), GetActorRotation());

	if (!GetWorldTimerManager().IsTimerActive(TurnHandle) && (AimRotator.Yaw > 90.0 || AimRotator.Yaw < -90.0 || AimRotator.Pitch > 90.0 || AimRotator.Pitch < -90.0))
	{
		TargetTurnRotation = GetControlRotation();
		StartTurnRotation = GetActorRotation();
		
		//GetWorldTimerManager().SetTimer(TurnHandle, this, &ABaseCharacter::Turn, 0.05f, true, 0.0f);
	}

	if (!GetMesh()->GetAnimInstance()->Montage_IsPlaying(nullptr))
	{
		if (PauseHitCounter > MaxPauseHitCounter)
		{
			GetMesh()->GetAnimInstance()->Montage_Resume(nullptr);
			PauseHitCounter = 0;
		}

		PauseHitCounter++;
	}

	if (GetMesh() != nullptr && HasTarget()) {
		SetActorRotation(FRotator(GetActorRotation().Pitch, UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target->GetActorLocation()).Yaw, GetActorRotation().Roll));
	}
	
	if (!CanPunch && !AttackBoneNames.IsEmpty()) {
		CheckFistCollision(AttackBoneNames[0]);
	}
}

void ABaseCharacter::Turn()
{
	TurnAlpha += 0.05;
	SetActorRotation(UKismetMathLibrary::RLerp(StartTurnRotation, FRotator(StartTurnRotation.Roll, TargetTurnRotation.Yaw, StartTurnRotation.Pitch), TurnAlpha, true));
		
	if (TurnAlpha >= 1.0f)
	{
		TurnAlpha = 0.0f;
		GetWorldTimerManager().ClearTimer(TurnHandle);
	}
}

void ABaseCharacter::UpdateLookAtTarget()
{
	
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABaseCharacter::OnCombatMontageEnd(UAnimMontage* Montage_, bool interrupted_) {
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("resetting isattacking")));
	IsAttacking = false;
	LastFistCollisionLocation = FVector::Zero();
	CurrActorsHit.Empty();
}

void ABaseCharacter::OnParryMontageEnd(UAnimMontage* Montage_, bool interrupted_)
{
	IsParrying = false;
}

void ABaseCharacter::OnHitReactionMontageEnd(UAnimMontage* Montage_, bool interrupted_)
{
	IsRecovering = false;
}

/*
float ABaseCharacter::PlayAnimMontage_Implementation(UAnimMontage* AnimMontage, float InPlayRate, FName StartSectionName)
{
	Super::PlayAnimMontage(AnimMontage, InPlayRate, StartSectionName);
} */


void ABaseCharacter::PlayHitReactionMontage_Implementation(const FVector& Location, const EKnockBackEnum KnockBack)
{
	
	const FTransform HeadTransform = GetMesh()->GetBoneTransform("head");
	const FTransform RootTransform = GetMesh()->GetBoneTransform("pelvis");
	const FTransform LegTransformL = GetMesh()->GetBoneTransform("calf_l");
	const FTransform LegTransformR = GetMesh()->GetBoneTransform("calf_r");
	FVector DiffFromHead = HeadTransform.GetLocation() - Location;
	FVector DiffFromRoot = RootTransform.GetLocation() - Location;
	FVector DiffFromLeg = ((LegTransformL.GetLocation() + LegTransformR.GetLocation()) / 2) - Location;

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("%f"), DiffFromHead.Length()));
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("%f"), DiffFromRoot.Length()));

	if (DiffFromHead.Length() * 1.5f < DiffFromRoot.Length())
	{
		DiffFromHead.Normalize();
		AttackedDir = DiffFromHead;
		AttackedBone = "head";
		const UE::Math::TQuat HeadRotation = HeadTransform.GetRotation();
		const float UpwardsDirection = DiffFromHead.Dot(HeadRotation.GetUpVector());
		const float SidewaysDirection = DiffFromHead.Dot(HeadRotation.GetRightVector());
		const float ForwardDirection = DiffFromHead.Dot(HeadRotation.GetForwardVector());

		if (SidewaysDirection >= 0.25f)
		{
			if (KnockBack == EKnockBackEnum::KE_NoKnockBack && HitReactionMontages.RightHeadHit != nullptr)
			{
				PlayAnimMontage(HitReactionMontages.RightHeadHit);
			}
			else if (KnockBack == EKnockBackEnum::KE_LightKnockBack && HitReactionMontages.RightHeadHitH != nullptr)
			{
				PlayAnimMontage(HitReactionMontages.RightHeadHitH);
			}
		}
		else if (SidewaysDirection <= -0.25f)
		{
			if (KnockBack == EKnockBackEnum::KE_NoKnockBack && HitReactionMontages.LeftHeadHit != nullptr)
			{
				PlayAnimMontage(HitReactionMontages.LeftHeadHit);
			}
			else if (KnockBack == EKnockBackEnum::KE_LightKnockBack && HitReactionMontages.LeftHeadHitH != nullptr)
			{
				PlayAnimMontage(HitReactionMontages.LeftHeadHitH);
			}
		}
		else
		{
			if (KnockBack == EKnockBackEnum::KE_NoKnockBack && HitReactionMontages.FrontHeadHit != nullptr)
			{
				PlayAnimMontage(HitReactionMontages.FrontHeadHit);
			}
			else if (KnockBack == EKnockBackEnum::KE_LightKnockBack && HitReactionMontages.FrontHeadHitH != nullptr)
			{
				PlayAnimMontage(HitReactionMontages.FrontHeadHitH);
			}
		}
	}
	else if (DiffFromLeg.Length() < DiffFromRoot.Length())
	{
		DiffFromRoot.Normalize();
		AttackedDir = DiffFromRoot;
		AttackedBone = "pelvis";
		
		const UE::Math::TQuat RootRotation = RootTransform.GetRotation();
		const float UpwardsDirection = DiffFromRoot.Dot(RootRotation.GetUpVector());
		const float SidewaysDirection = DiffFromRoot.Dot(RootRotation.GetRightVector());
		const float ForwardDirection = DiffFromRoot.Dot(RootRotation.GetForwardVector());

		if (SidewaysDirection >= 0.5f)
		{
			if (KnockBack == EKnockBackEnum::KE_NoKnockBack && HitReactionMontages.RightLegHit != nullptr)
			{
				PlayAnimMontage(HitReactionMontages.RightLegHit);
			}
			else if (KnockBack == EKnockBackEnum::KE_LightKnockBack && HitReactionMontages.RightLegHitH != nullptr)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("hit righth")));
				PlayAnimMontage(HitReactionMontages.RightLegHitH);
			}
		}
		else
		{
			if (KnockBack == EKnockBackEnum::KE_NoKnockBack && HitReactionMontages.LeftLegHit != nullptr)
			{
				PlayAnimMontage(HitReactionMontages.LeftLegHit);
			}
			else if (KnockBack == EKnockBackEnum::KE_LightKnockBack  && HitReactionMontages.LeftLegHitH != nullptr)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("hit lefth")));
				PlayAnimMontage(HitReactionMontages.LeftLegHitH);
			}
		}
	}
	else
	{
		DiffFromLeg.Normalize();
		AttackedDir = DiffFromLeg;
		AttackedBone = "calf_l";
		
		const UE::Math::TQuat RootRotation = RootTransform.GetRotation();
		DiffFromRoot.Normalize();
		const float UpwardsDirection = DiffFromRoot.Dot(RootRotation.GetUpVector());
		const float SidewaysDirection = DiffFromRoot.Dot(RootRotation.GetRightVector());
		const float ForwardDirection = DiffFromRoot.Dot(RootRotation.GetForwardVector());

		if (SidewaysDirection >= 0.5f)
		{
			if (KnockBack == EKnockBackEnum::KE_NoKnockBack && HitReactionMontages.RightHit != nullptr)
			{
				PlayAnimMontage(HitReactionMontages.RightHit);
			}
			else if (KnockBack == EKnockBackEnum::KE_LightKnockBack  && HitReactionMontages.RightHitH != nullptr)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("hit righth")));
				PlayAnimMontage(HitReactionMontages.RightHitH);
			}
		}
		else if (SidewaysDirection <= -0.5f)
		{
			if (KnockBack == EKnockBackEnum::KE_NoKnockBack && HitReactionMontages.LeftHit != nullptr)
			{
				PlayAnimMontage(HitReactionMontages.LeftHit);
			}
			else if (KnockBack == EKnockBackEnum::KE_LightKnockBack  && HitReactionMontages.LeftHitH != nullptr)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("hit lefth")));
				PlayAnimMontage(HitReactionMontages.LeftHitH);
			}
		}
		else
		{
			if (KnockBack == EKnockBackEnum::KE_NoKnockBack && HitReactionMontages.FrontHit != nullptr)
			{
				PlayAnimMontage(HitReactionMontages.FrontHit);
			}
			else if (KnockBack == EKnockBackEnum::KE_LightKnockBack && HitReactionMontages.FrontHitH != nullptr)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("hit fronth")));
				PlayAnimMontage(HitReactionMontages.FrontHitH);
			}
		}
	}

	if (GetMesh()->GetAnimInstance()->Montage_IsActive(nullptr)) {
		FOnMontageEnded EndDelegate;
		        
		EndDelegate.BindUObject(this, &ABaseCharacter::OnHitReactionMontageEnd);
		        
		GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(EndDelegate);

		// GetMesh()->GetAnimInstance()->Montage_Pause();
	}
}

UStat* ABaseCharacter::GetMovementSpeed()
{
	return StatMap.FindOrAdd(EStatEnum::SE_MovementSpeed);
}


void ABaseCharacter::SetMovementSpeed(UStat* Stat)
{
	if (const UStat* MovementSpeed = StatMap.FindOrAdd(EStatEnum::SE_MovementSpeed))
	{
		MovementSpeed = Stat;
		UpdateMovementSpeed();
	}
}

void ABaseCharacter::UpdateMovementSpeed()
{
	if (const UStat* MovementSpeed = StatMap.FindOrAdd(EStatEnum::SE_MovementSpeed))
	{
		GetCharacterMovement()->MaxWalkSpeed = MovementSpeed->GetData();
	}
}



void ABaseCharacter::ActivateAbility(const int AbilityIndex)
{
	if (AbilitySystem != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("ability activate attempt")));
		AbilitySystem->ActivateAbility(AbilityIndex, this);
	}
}

void ABaseCharacter::EndAbilityActivation(const int AbilityIndex)
{
	if (AbilitySystem != nullptr)
	{
		AbilitySystem->EndAbilityActivation(AbilityIndex);
	}
}


void ABaseCharacter::AddMovementSpeedModifier(UStatModifier* StatModifier)
{
	UStat* MovementSpeed = StatMap.FindOrAdd(EStatEnum::SE_MovementSpeed);
	
	if (StatModifier != nullptr && MovementSpeed != nullptr)
	{
		StatModifier->SetParentStat(MovementSpeed);
		SetMovementSpeed(StatModifier);
	}
}

void ABaseCharacter::CheckFistCollision(FName BoneName) {
	FTransform HandBone = GetMesh()->GetBoneTransform(BoneName);

	const FVector End = HandBone.GetLocation();

	FVector Start = End;

	if (!LastFistCollisionLocation.IsZero())
	{
		Start = LastFistCollisionLocation;
	}

	LastFistCollisionLocation = End;

	FHitResult HitResult;

	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjects;

	TraceObjects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1));

	CurrActorsHit.Add(this);

	const bool Hit = UKismetSystemLibrary::SphereTraceSingleForObjects(
		GetWorld(), 
		Start, 
		End, 
		FistCollisionTraceRadius,
		TraceObjects,
		false,
		CurrActorsHit,
		EDrawDebugTrace::ForDuration,
		HitResult,
		true);

	if (Hit) {
		AActor* HitActor = HitResult.GetActor();

		CurrActorsHit.Add(HitActor);
	
		if (ABaseCharacter* HitEnemy = Cast<ABaseCharacter>(HitActor)) {
			HitEnemy->HandleHit(Start, HitResult.ImpactPoint, AttackLevel, EquippedWeapon);
		}
	
		GetMesh()->GetAnimInstance()->Montage_Pause();
    
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Hit %s"), *HitActor->GetName()));
	}
}

void ABaseCharacter::SetHeavyAttack_Implementation()
{
	if (MainAttackIsCharging)
	{
		AttackLevel = EAttackLevelEnum::AE_HeavyAttack;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("set attack level heavy")));
		MainAttackIsCharging = false;
	}
}


void ABaseCharacter::IncrementParryCounter(const int MaxCounterVal)
{
	if (IsParrying)
	{
		if (ParryCounter < MaxCounterVal)
		{
			ParryCounter++;
		}
		else
		{
			ParryCounter = 0;
			IsParrying = false;
		}
	}
}


void ABaseCharacter::HandleHit(const FVector& PreHitLocation, const FVector& HitLocation, const EAttackLevelEnum AttackLevelI, UWeapon* Weapon)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("hit")));
	
	if (IsParrying || IsDodging)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("hit in iFrame")));
		PlayParryMontage(FMath::RandRange(0, ParryMontages.Num() - 1));
	}
	else
	{
		IsRecovering = true;
		CanPunch = true;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("hit valid")));
		
		if (Weapon != nullptr)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Weapon->HitEffect, HitLocation, UKismetMathLibrary::FindLookAtRotation(HitLocation, PreHitLocation), FVector::One() * Weapon->HitParticleSize);

			if (Weapon->GetStaminaDamage() != nullptr)
			{
				AddCurrStamina(Weapon->GetStaminaDamage()->GetData());
			}
			
			if (AttackLevelI == EAttackLevelEnum::AE_LightAttack && Weapon->GetLightDamage() != nullptr)
			{
				PlayHitReactionMontage(PreHitLocation, Weapon->GetKnockBack());
				AddCurrHealth(Weapon->GetLightDamage()->GetData());
				
			}
			else if (AttackLevelI == EAttackLevelEnum::AE_HeavyAttack && Weapon->GetHeavyDamage() != nullptr)
			{
				PlayHitReactionMontage(PreHitLocation, Weapon->GetHeavyKnockBack());
				AddCurrHealth(Weapon->GetHeavyDamage()->GetData());
			}
		}
	}
}

void ABaseCharacter::AddCurrHealth(const float HealthAddition)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("weapon damage taken %f"), HealthAddition));
	UStat* Health = StatMap.FindOrAdd(EStatEnum::SE_Health);
	const UStat* MaxHealth = StatMap.FindOrAdd(EStatEnum::SE_MaxHealth);
	
	if (Health != nullptr && MaxHealth != nullptr && Health->GetData() + HealthAddition <= MaxHealth->GetData())
	{
		Health->AddData(HealthAddition);

		UpdateHealthUI();
	}
	
	HandleDeath();
}

void ABaseCharacter::UpdateHealthUI()
{
	const UStat* Health = StatMap.FindOrAdd(EStatEnum::SE_Health);
	const UStat* MaxHealth = StatMap.FindOrAdd(EStatEnum::SE_MaxHealth);
	
	if (Health != nullptr && MaxHealth != nullptr && StatDisplay != nullptr)
	{
		StatDisplay->DisplayHealthPercentage(Health->GetData(), MaxHealth->GetData());
	}
}


void ABaseCharacter::AddCurrStamina(const float StaminaAddition)
{
	UStat* Stamina = StatMap.FindOrAdd(EStatEnum::SE_Stamina);
	const UStat* MaxStamina = StatMap.FindOrAdd(EStatEnum::SE_MaxStamina);
	
	if (Stamina  != nullptr && MaxStamina != nullptr && Stamina->GetData() + StaminaAddition <= MaxStamina->GetData())
	{
		Stamina->AddData(StaminaAddition);

		UpdateStaminaUI();
	}	
}

void ABaseCharacter::UpdateStaminaUI()
{
	const UStat* Stamina = StatMap.FindOrAdd(EStatEnum::SE_Stamina);
	const UStat* MaxStamina = StatMap.FindOrAdd(EStatEnum::SE_MaxStamina);
	
	if (Stamina  != nullptr && MaxStamina != nullptr && StatDisplay != nullptr)
	{
		StatDisplay->DisplayStaminaPercentage(Stamina->GetData(), MaxStamina->GetData());
	}	
}



void ABaseCharacter::HandleDeath()
{
	if (CheckIfDead())
	{
		CleanupOnDeath();
		// Destroy();
	}
}

void ABaseCharacter::CleanupOnDeath()
{
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetSimulatePhysics(true);
	GetCharacterMovement()->DisableMovement();
	GetMesh()->AddImpulse(AttackedDir * AttackedForce, AttackedBone, true);
}


bool ABaseCharacter::CheckIfDead()
{
	if (const UStat* Health = StatMap.FindOrAdd(EStatEnum::SE_Health))
	{
		return Health->GetData() <= 0.0f;
	}

	return false;
}

void ABaseCharacter::Attack_Implementation(bool IsAltAttack) {
	if (Controller != nullptr && CanAct() && AbilitySystem != nullptr) {
		UAbility* EquippedAbility = AbilitySystem->GetMainEquippedAbility();
		
		if (EquippedAbility != nullptr)
		{
			EquippedAbility->UseMainAction();
		}
		else
		{
			// IsLastAltAttack = IsAltAttacking;
			// IsAltAttacking = IsAltAttack;
			AttackLevel = EAttackLevelEnum::AE_LightAttack;
			MainAttackIsCharging = true;
			
			CanPunch = false;
			IsAttacking = true;
			AttackBoneNames.Empty();
			
			const FVector Start = GetActorLocation() + (GetCapsuleComponent()->GetForwardVector() * 50);
			const FVector End = Start;

			TArray<AActor*> ActorsToIgnore;

			ActorsToIgnore.Add(this);

			FHitResult HitResult;

			TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjects;

			TraceObjects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1));

			const bool Hit = UKismetSystemLibrary::BoxTraceSingleForObjects(
				GetWorld(), 
				Start, 
				End, 
				FVector(MainAttackTraceRadius), 
				Controller->GetControlRotation(), 
				TraceObjects, 
				false, 
				ActorsToIgnore, 
				EDrawDebugTrace::ForDuration,
				HitResult,
				true);

			if (Hit) {
				Target = Cast<ABaseCharacter>(HitResult.GetActor());
				
				// MotionWarpingComponent->AddOrUpdateWarpTargetFromTransform("AttackTarget", Target->GetTransform());
				// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Hit %s"), *Target->GetName()));
			}
			else
			{
				// MotionWarpingComponent->AddOrUpdateWarpTargetFromLocation("AttackTarget", GetActorLocation() + (GetActorForwardVector() * 100.0f));
				// MotionWarpingComponent->RemoveWarpTarget("AttackTarget");
			}

			LastFistCollisionLocation = FVector::Zero();

			PlayAttackMontage(IsAltAttack);
			//GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &AMyCharacter::OnPunchingMontageEnd);
		}
	}  
}

void ABaseCharacter::AttackRelease_Implementation()
{
	if (AbilitySystem != nullptr)
	{
		UAbility* EquippedAbility = AbilitySystem->GetMainEquippedAbility();
		
		if (EquippedAbility != nullptr)
		{
			EquippedAbility->EndMainAction();
		}
		else
		{
			MainAttackIsCharging = false;
			GetMesh()->GetAnimInstance()->Montage_SetPlayRate(nullptr, 1.0f);
		}
	}
}


void ABaseCharacter::Block_Implementation()
{
	if (CanAct())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("block started")));
		IsParrying = true;
		IsBlocking = true;
		// PlayParryMontage(FMath::RandRange(0, ParryMontages.Num() - 1));
	}
}

void ABaseCharacter::EndBlock_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("block ended")));
	IsBlocking = false;
}

void ABaseCharacter::Dodge_Implementation(FVector Direction)
{
	if (CanAct())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("block started")));
		IsDodging = true;

		FVector DodgeDirection = FVector(FVector::DotProduct(GetActorForwardVector(), GetCharacterMovement()->GetCurrentAcceleration()),
			FVector::DotProduct(GetActorRightVector(), GetCharacterMovement()->GetCurrentAcceleration()),
			0.0f);
		
		DodgeDirection.Normalize();
		
		PlayDodgeMontage(DodgeDirection);
	}
}

void ABaseCharacter::PlayDodgeMontage_Implementation(FVector Direction)
{
	// const int RandNum = FMath::RandRange(0, 2);
	if (Direction.Y > 0.5f && DodgeMontages.LeftDodge != nullptr)
	{
		ACharacter::PlayAnimMontage(DodgeMontages.LeftDodge);
	}
	else if (Direction.Y < -0.5f && DodgeMontages.RightDodge != nullptr)
	{
		ACharacter::PlayAnimMontage(DodgeMontages.RightDodge);
	}
	else if (Direction.X < -0.5f  && DodgeMontages.BackDodge != nullptr)
	{
		ACharacter::PlayAnimMontage(DodgeMontages.BackDodge);
	}
	else if (Direction.X > 0.5f && DodgeMontages.ForwardDodge != nullptr)
	{
		ACharacter::PlayAnimMontage(DodgeMontages.ForwardDodge);
	}
	else if (DodgeMontages.NeutralDodge != nullptr)
	{
		ACharacter::PlayAnimMontage(DodgeMontages.NeutralDodge);
	}

	if (GetMesh()->GetAnimInstance()->Montage_IsActive(nullptr)) {
		FOnMontageEnded EndDelegate;
		        
		EndDelegate.BindUObject(this, &ABaseCharacter::OnDodgeMontageEnd);
		        
		GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(EndDelegate);
		
		FVector AccDirection = GetCharacterMovement()->GetCurrentAcceleration();
		AccDirection.Normalize();
		
		DodgeDir = AccDirection;
		GetCharacterMovement()->MaxWalkSpeed = 1000.0f;
		
		// MotionWarpingComponent->AddOrUpdateWarpTargetFromLocation("DodgeWarp", GetActorLocation() + (AccDirection * 300.0f));
	}
}

void ABaseCharacter::OnDodgeMontageEnd(UAnimMontage* Montage_, bool interrupted_)
{
	UpdateMovementSpeed();
	DodgeTaper = 1.0f;
	IsDodging = false;
}

void ABaseCharacter::PlayParryMontage_Implementation(int Index)
{
	if (!ParryMontages.IsEmpty() && Index < ParryMontages.Num())
	{
		ACharacter::PlayAnimMontage(ParryMontages[Index]);

		FOnMontageEnded EndDelegate;
        
		EndDelegate.BindUObject(this, &ABaseCharacter::OnParryMontageEnd);
        
		GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(EndDelegate);
	}
}


void ABaseCharacter::PlayAttackMontage_Implementation(const bool IsAltAttack) {
	if (EquippedWeapon != nullptr)
	{
		UAnimMontage* AttackMontage = EquippedWeapon->GetNextAttackMontage(
			this,
			IsAltAttack,
			EAttackLevelEnum::AE_LightAttack,
			!(Target != nullptr && GetDistanceTo(Target) < (GetCapsuleComponent()->GetScaledCapsuleRadius() * 2) + 1));

		if (AttackMontage != nullptr)
		{
			ACharacter::PlayAnimMontage(AttackMontage, HeavyCombatMontageSpeed);

			FOnMontageEnded EndDelegate;
			        
			EndDelegate.BindUObject(this, &ABaseCharacter::OnCombatMontageEnd);
			        
			GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(EndDelegate);
		}
	}
}

bool ABaseCharacter::CanAct()
{
	if (CanPunch && !IsParrying && !IsDodging && !IsRecovering && !IsBlocking)
	{
		return true;
	}

	return false;
}

bool ABaseCharacter::HasTarget()
{
	return Target != nullptr;
}


void ABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseCharacter, Target);
	DOREPLIFETIME(ABaseCharacter, CanPunch);
}

/*
void ABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	
	DOREPLIFETIME(ABaseCharacter, Health);
	DOREPLIFETIME(ABaseCharacter, Mana);
	DOREPLIFETIME(ABaseCharacter, MovementSpeed);
	DOREPLIFETIME(ABaseCharacter, AbilitySystem);
	
}
*/

