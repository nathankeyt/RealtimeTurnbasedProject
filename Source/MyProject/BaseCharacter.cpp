// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "Stat.h"
#include "AbilitySystemComponent.h"
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
	IsPunching = false;
	CurrPunchHits.Empty();
}

void ABaseCharacter::PlayHitReaction_Implementation()
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


void ABaseCharacter::AddMovementSpeedModifier(UStatModifier* StatModifier)
{
	if (StatModifier != nullptr && MovementSpeed != nullptr)
	{
		StatModifier->SetParentStat(MovementSpeed);
		SetMovementSpeed(StatModifier);
	}
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

