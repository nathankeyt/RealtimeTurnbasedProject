// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "StatModifier.h"
#include "Components/CapsuleComponent.h"
#include "AbilitySystemComponent.h"
#include "ComboNode.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"

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

void ABaseCharacter::PlayHitReactionMontage_Implementation(const FVector& Location)
{
	if (HitReactionMontages.FrontHit != nullptr && HitReactionMontages.FrontHeadHit != nullptr) {
		const FTransform HeadTransform = GetMesh()->GetBoneTransform("head");
		const FTransform RootTransform = GetMesh()->GetBoneTransform("pelvis");
		FVector DiffFromHead = HeadTransform.GetLocation() - Location;
		FVector DiffFromRoot = RootTransform.GetLocation() - Location;

		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("%f"), DiffFromHead.Length()));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("%f"), DiffFromRoot.Length()));

		if (DiffFromHead.Length() < DiffFromRoot.Length())
		{
			const UE::Math::TQuat HeadRotation = HeadTransform.GetRotation();
			DiffFromHead.Normalize();
			const float UpwardsDirection = DiffFromHead.Dot(HeadRotation.GetUpVector());
			const float SidewaysDirection = DiffFromHead.Dot(HeadRotation.GetRightVector());
			const float ForwardDirection = DiffFromHead.Dot(HeadRotation.GetForwardVector());

			if (SidewaysDirection >= 0.25f && HitReactionMontages.RightHeadHit != nullptr)
			{ 
				PlayAnimMontage(HitReactionMontages.RightHeadHit);
			}
			else if (SidewaysDirection <= -0.25f && HitReactionMontages.LeftHeadHit != nullptr)
			{
				PlayAnimMontage(HitReactionMontages.LeftHeadHit);
			}
			else
			{
				PlayAnimMontage(HitReactionMontages.FrontHeadHit);
			}
		}
		else
		{
			const UE::Math::TQuat RootRotation = RootTransform.GetRotation();
			DiffFromRoot.Normalize();
			const float UpwardsDirection = DiffFromRoot.Dot(RootRotation.GetUpVector());
			const float SidewaysDirection = DiffFromRoot.Dot(RootRotation.GetRightVector());
			const float ForwardDirection = DiffFromRoot.Dot(RootRotation.GetForwardVector());

			if (SidewaysDirection >= 0.5f && HitReactionMontages.RightHit != nullptr)
			{ 
				PlayAnimMontage(HitReactionMontages.RightHit);
			}
			else if (SidewaysDirection <= -0.5f && HitReactionMontages.LeftHit != nullptr)
			{
				PlayAnimMontage(HitReactionMontages.LeftHit);
			}
			else
			{
				PlayAnimMontage(HitReactionMontages.FrontHit);
			}
		}
		
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
	AbilitySystem->ActivateAbility(AbilityIndex, this);
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
		
		ABaseCharacter* HitEnemy = Cast<ABaseCharacter>(HitActor);
		
		if (HitEnemy != nullptr) {
			HitEnemy->HandleHit(Start);
		}
		
		GetMesh()->GetAnimInstance()->Montage_Pause();
        
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Hit %s"), *HitActor->GetName()));
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


void ABaseCharacter::HandleHit(const FVector& HitLocation)
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
		PlayHitReactionMontage(HitLocation);
	}
}


void ABaseCharacter::MainAttack_Implementation() {
	if (Controller != nullptr && CanAct()) {
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
			Target = HitResult.GetActor();
			MotionWarpingComponent->AddOrUpdateWarpTargetFromTransform("AttackTarget", Target->GetTransform());
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Hit %s"), *Target->GetName()));
		}
		else
		{
			MotionWarpingComponent->AddOrUpdateWarpTargetFromLocation("AttackTarget", GetActorLocation() + (GetActorForwardVector() * 100.0f));
			// MotionWarpingComponent->RemoveWarpTarget("AttackTarget");
		}

		LastFistCollisionLocation = FVector::Zero();

		PlayMainAttackMontage(FMath::RandRange(0, CombatMontages.Num() - 1));
		//GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &AMyCharacter::OnPunchingMontageEnd);
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

void ABaseCharacter::Dodge_Implementation()
{
	if (CanAct())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("block started")));
		IsDodging = true;
		
		PlayDodgeMontage(FVector(FVector::DotProduct(GetActorForwardVector(),  GetVelocity()), FVector::DotProduct(GetActorRightVector(), GetVelocity()), 0.0f));
	}
}

void ABaseCharacter::PlayDodgeMontage_Implementation(FVector Direction)
{
	if (DodgeMontages.BackDodge != nullptr || DodgeMontages.LeftDodge != nullptr || DodgeMontages.RightDodge != nullptr) {
		const int RandNum = FMath::RandRange(0, 2);
		if (RandNum == 0) //Direction.Y < 0.0f)
		{
			ACharacter::PlayAnimMontage(DodgeMontages.LeftDodge);
		}
		else if (RandNum == 1)//Direction.Y > 0.0f)
		{
			ACharacter::PlayAnimMontage(DodgeMontages.RightDodge);
		}
		else
		{
			ACharacter::PlayAnimMontage(DodgeMontages.BackDodge);
		}
	
		FOnMontageEnded EndDelegate;
		        
		EndDelegate.BindUObject(this, &ABaseCharacter::OnDodgeMontageEnd);
		        
		GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(EndDelegate);
	}
}

void ABaseCharacter::OnDodgeMontageEnd(UAnimMontage* Montage_, bool interrupted_)
{
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



void ABaseCharacter::PlayMainAttackMontage_Implementation(int Index) {
	if (!CombatMontages.IsEmpty() && Index < CombatMontages.Num())
	{
		if (ComboCounter == 0 || CurrComboNode == nullptr)
		{
			CurrComboNode = CombatMontages[Index];
		}
		else if (ComboCounter == 2)
		{
			ComboCounter = -1;
			CurrComboNode = CurrComboNode->End;
		}
		else
		{
			CurrComboNode = CurrComboNode->Next;
		}

		ComboCounter++;

		if (CurrComboNode != nullptr && CurrComboNode->CurrMontage != nullptr)
		{
			ACharacter::PlayAnimMontage(CurrComboNode->CurrMontage, 1.0f);
			
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

