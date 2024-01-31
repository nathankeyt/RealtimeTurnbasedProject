// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"

#include "PlayerCharacter.h"
#include "StatWidget.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("StatWidgetComponent"));
	WidgetComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	// comment test
	if (WidgetComponent != nullptr && StatDisplay != nullptr)
	{
		WidgetComponent->SetWidget(StatDisplay);
	}

	Super::BeginPlay();
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const APlayerCharacter* Player = Cast<APlayerCharacter>(GEngine->GetFirstLocalPlayerController(GetWorld())->GetCharacter());

	if (WidgetComponent != nullptr && Player != nullptr) {
		WidgetComponent->SetWorldRotation(FRotator(WidgetComponent->GetComponentRotation().Pitch, UKismetMathLibrary::FindLookAtRotation(WidgetComponent->GetComponentLocation(), Player->GetActorLocation()).Yaw, WidgetComponent->GetComponentRotation().Roll));
	} 
	
}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent (PlayerInputComponent);

}

void AEnemyCharacter::CleanupOnDeath()
{
	Super::CleanupOnDeath();

	WidgetComponent->DestroyComponent();
}



