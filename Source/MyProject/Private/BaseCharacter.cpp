// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "StatModifier.h"
#include "Components/CapsuleComponent.h"
#include "AbilitySystemComponent.h"
#include "ComboNode.h"
#include "EquipAbilityBase.h"
#include "KnockbackEnum.h"
#include "StatWidget.h"
#include "Weapon.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Physics/ImmediatePhysics/ImmediatePhysicsShared/ImmediatePhysicsCore.h"

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

	/*
	if (IsRecovering)
	{
		if (RecoveringCounter < 100)
		{
			RecoveringCounter++;
		}
		else
		{
			RecoveringCounter = 0;
			IsRecovering = false;
		} 
	} */

	if (IsDodging && GetMesh()->GetAnimInstance()->Montage_IsActive(nullptr) && DodgeTaper > 0.0f)
	{
		AddMovementInput(DodgeDir, DodgeTaper);
		DodgeTaper -= 0.005f;
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
		AttackedDir = DiffFromHead;
		AttackedBone = "head";
		const UE::Math::TQuat HeadRotation = HeadTransform.GetRotation();
		DiffFromHead.Normalize();
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
		AttackedDir = DiffFromRoot;
		AttackedBone = "pelvis";
		
		const UE::Math::TQuat RootRotation = RootTransform.GetRotation();
		DiffFromRoot.Normalize();
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
	}
}

void ABaseCharacter::SetMovementSpeed(UStat* Stat)
{
	if (Stat != nullptr)
	{
		MovementSpeed = Stat;
		UpdateMovementSpeed();
	}
}

void ABaseCharacter::UpdateMovementSpeed()
{
	GetCharacterMovement()->MaxWalkSpeed = MovementSpeed->GetData();
}

void ABaseCharacter::ActivateAbility(const int AbilityIndex)
{
	if (AbilitySystem != nullptr)
	{
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
			HitEnemy->HandleHit(Start, AttackLevel, EquippedWeapon);
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


void ABaseCharacter::HandleHit(const FVector& HitLocation, const EAttackLevelEnum AttackLevelI, UWeapon* Weapon)
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
			if (Weapon->GetStaminaDamage() != nullptr)
			{
				AddCurrStamina(Weapon->GetStaminaDamage()->GetData());
			}
			
			if (AttackLevelI == EAttackLevelEnum::AE_LightAttack && Weapon->GetDamage() != nullptr)
			{
				PlayHitReactionMontage(HitLocation, Weapon->GetKnockBack());
				AddCurrHealth(Weapon->GetDamage()->GetData());
				
			}
			else if (AttackLevelI == EAttackLevelEnum::AE_HeavyAttack && Weapon->GetHeavyDamage() != nullptr)
			{
				PlayHitReactionMontage(HitLocation, Weapon->GetHeavyKnockBack());
				AddCurrHealth(Weapon->GetHeavyDamage()->GetData());
			}
		}
	}
}

void ABaseCharacter::AddCurrHealth(const float HealthAddition)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("weapon damage taken %f"), HealthAddition));
	if (CurrHealth != nullptr)
	{
		CurrHealth->AddData(HealthAddition);

		UpdateHealthUI();
	}
	
	HandleDeath();
}

void ABaseCharacter::UpdateHealthUI()
{
	if (CurrHealth != nullptr && MaxHealth != nullptr && StatDisplay != nullptr)
	{
		StatDisplay->DisplayHealthPercentage(CurrHealth->GetData(), MaxHealth->GetData());
	}
}


void ABaseCharacter::AddCurrStamina(const float StaminaAddition)
{
	if (CurrStamina != nullptr)
	{
		CurrStamina->AddData(StaminaAddition);

		UpdateStaminaUI();
	}	
}

void ABaseCharacter::UpdateStaminaUI()
{
	if (CurrStamina != nullptr && MaxStamina != nullptr && StatDisplay != nullptr)
	{
		StatDisplay->DisplayStaminaPercentage(CurrStamina->GetData(), MaxStamina->GetData());
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


bool ABaseCharacter::CheckIfDead() const
{
	if (CurrHealth != nullptr)
	{
		return CurrHealth->GetData() <= 0.0f;
	}

	return false;
}




void ABaseCharacter::MainAttack_Implementation(bool IsAltAttack) {
	if (Controller != nullptr && CanAct()) {
		UEquipAbilityBase* EquippedAbility = AbilitySystem->GetMainEquippedAbility();
		
		if (EquippedAbility != nullptr)
		{
			EquippedAbility->Charge();
		}
		else
		{
			IsLastAltAttack = IsAltAttacking;
			IsAltAttacking = IsAltAttack;
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

			PlayMainAttackMontage(IsLastAltAttack != IsAltAttacking);
			//GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &AMyCharacter::OnPunchingMontageEnd);
		}
	}  
}

void ABaseCharacter::MainAttackRelease_Implementation()
{
	UEquipAbilityBase* EquippedAbility = AbilitySystem->GetMainEquippedAbility();
		
	if (EquippedAbility != nullptr)
	{
		EquippedAbility->Fire();
	}
	else
	{
		MainAttackIsCharging = false;
		GetMesh()->GetAnimInstance()->Montage_SetPlayRate(nullptr, 1.0f);
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

UComboNode* ABaseCharacter::GetRandomComboStart()
{
	if (IsAltAttacking)
	{
		return AltCombatMontages[FMath::RandRange(0, AltCombatMontages.Num() - 1)];
	}

	return CombatMontages[FMath::RandRange(0, CombatMontages.Num() - 1)];
}


void ABaseCharacter::PlayMainAttackMontage_Implementation(const bool ShouldResetCombo) {
	if (!CombatMontages.IsEmpty() && !AltCombatMontages.IsEmpty())
	{
		if (ShouldResetCombo || CurrComboNode == nullptr || CurrComboNode->Next == nullptr)
		{
			CurrComboNode = GetRandomComboStart();
		}
		else
		{
			CurrComboNode = CurrComboNode->Next;
		}

		if (CurrComboNode != nullptr)
		{
			if (Target != nullptr && GetDistanceTo(Target) < (GetCapsuleComponent()->GetScaledCapsuleRadius() * 2) + 1 && CurrComboNode->CurrMontageIP != nullptr)
			{
				ACharacter::PlayAnimMontage(CurrComboNode->CurrMontageIP, HeavyCombatMontageSpeed);
					
				FOnMontageEnded EndDelegate;
			        
				EndDelegate.BindUObject(this, &ABaseCharacter::OnCombatMontageEnd);
			        
				GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(EndDelegate);
			}
			else if (CurrComboNode->CurrMontageM != nullptr)
			{
				ACharacter::PlayAnimMontage(CurrComboNode->CurrMontageM, HeavyCombatMontageSpeed);
					
				FOnMontageEnded EndDelegate;
			        
				EndDelegate.BindUObject(this, &ABaseCharacter::OnCombatMontageEnd);
			        
				GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(EndDelegate);
			}
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

