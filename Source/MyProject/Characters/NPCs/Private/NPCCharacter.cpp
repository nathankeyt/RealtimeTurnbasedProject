// Fill out your copyright notice in the Description page of Project Settings.


#include "MyProject/Characters/NPCs/Public/NPCCharacter.h"

#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "MyProject/Characters/Player/Public/PlayerCharacter.h"
#include "MyProject/Stats/UI/Public/StatWidget.h"

// Sets default values
ANPCCharacter::ANPCCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("StatWidgetComponent"));
	WidgetComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ANPCCharacter::BeginPlay()
{
	// comment test
	if (WidgetComponent != nullptr && StatDisplay != nullptr)
	{
		WidgetComponent->SetWidget(StatDisplay);
	}

	if (const APlayerController* PlayerController = GEngine->GetFirstLocalPlayerController(GetWorld()))
	{
		PlayerCharacter = Cast<APlayerCharacter>(PlayerController->GetCharacter());
	}

	Super::BeginPlay();
}

// Called every frame
void ANPCCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (WidgetComponent != nullptr && PlayerCharacter != nullptr) {
		WidgetComponent->SetWorldRotation(FRotator(WidgetComponent->GetComponentRotation().Pitch, UKismetMathLibrary::FindLookAtRotation(WidgetComponent->GetComponentLocation(), PlayerCharacter->GetActorLocation()).Yaw, WidgetComponent->GetComponentRotation().Roll));
	}

	if (ShouldScanForPlayer)
	{
		ScanForPlayer();
	}
}

void ANPCCharacter::SetShouldScanForPlayer(bool ShouldScan)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("set should scan for player")));
	
	if (!ShouldScan && PlayerCharacter != nullptr)
	{
		PlayerCharacter->ShouldScanForNPC = false;
	}

	ShouldScanForPlayer = ShouldScan;
}


void ANPCCharacter::ScanForPlayer()
{
	if ((GetActorLocation() - PlayerCharacter->GetActorLocation()).Length() < ScanRadius)
	{
		if (PlayerCharacter != nullptr && !PlayerCharacter->ShouldScanForNPC)
		{
			PlayerCharacter->SetShouldScanForNPC(true);
		}
	}
	else if (PlayerCharacter->ShouldScanForNPC)
	{
		PlayerCharacter->SetShouldScanForNPC(false);
	}
}


void ANPCCharacter::CleanupOnDeath()
{
	Super::CleanupOnDeath();

	WidgetComponent->DestroyComponent();
}



