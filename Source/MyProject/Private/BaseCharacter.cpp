// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "StatModifier.h"
#include "Components/CapsuleComponent.h"
#include "AbilitySystemComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	AbilitySystem = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem"));
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

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseCharacter::OnPunchingMontageEnd(UAnimMontage* Montage_, bool interrupted_) {
	IsAttacking = false;
	CurrActorsHit.Empty();
}

void ABaseCharacter::PlayHitReactionMontage_Implementation()
{
	if (!HitReactionMontages.IsEmpty())
	{
		PlayAnimMontage(HitReactionMontages[FMath::RandRange(0, HitReactionMontages.Num() - 1)]);
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
	const FVector Start = HandBone.GetLocation();
	const FVector End = Start;

	FHitResult HitResult;

	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjects;

	TraceObjects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1));

	const bool Hit = UKismetSystemLibrary::SphereTraceSingleForObjects(
		GetWorld(), 
		Start, 
		End, 
		5.0,
		TraceObjects,
		false,
		CurrActorsHit,
		EDrawDebugTrace::ForDuration,
		HitResult,
		true);

	if (Hit) {
		AActor* HitActor = HitResult.GetActor();
		ABaseCharacter* HitEnemy = Cast<ABaseCharacter>(HitActor);

		CurrActorsHit.Add(HitActor);

		if (HitEnemy != nullptr) {
			HitEnemy->PlayHitReactionMontage();
		}
        
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Hit %s"), *HitActor->GetName()));
	}
}

void ABaseCharacter::MainAttack_Implementation() {
	if (Controller != nullptr && CanPunch) {
		CanPunch = false;
		IsAttacking = true;
		
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
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Hit %s"), *Target->GetName()));
		}

		PlayMainAttackMontage();
		//GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &AMyCharacter::OnPunchingMontageEnd);
	}  
}

void ABaseCharacter::PlayMainAttackMontage_Implementation() {
	ACharacter::PlayAnimMontage(CombatMontages[FMath::RandRange(0, CombatMontages.Num() - 1)]); // implement random num gen on server

	FOnMontageEnded EndDelegate;
        
	EndDelegate.BindUObject(this, &ABaseCharacter::OnPunchingMontageEnd);
        
	GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(EndDelegate);
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

