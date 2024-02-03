// Fill out your copyright notice in the Description page of Project Settings.


#include "MyProject/Characters/Player/Public/PlayerCharacter.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Logging/LogMacros.h"
#include "EnhancedInputComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MyProject/AbilitySystem/UI/Public/AbilityDisplayWidget.h"
#include "MyProject/Stats/UI/Public/StatWidget.h"


DEFINE_LOG_CATEGORY(LogTemplateCharacter);

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// PrimaryActorTick.bCanEverTick = true;
    GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;
    
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);

    GetCharacterMovement()->JumpZVelocity = 600.0f;
    GetCharacterMovement()->AirControl = 0.2f;
    GetCharacterMovement()->MaxWalkSpeed = 500.0f;
    GetCharacterMovement()->MinAnalogWalkSpeed = 20.0f;
    GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;
    GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
    
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    
    CameraBoom->TargetArmLength = 300.0f;
    CameraBoom->bUsePawnControlRotation = true;
    
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    
    FollowCamera->bUsePawnControlRotation = false;

    TargetAimOffset = FVector::Zero();
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
    if (StatDisplay != nullptr)
    {
        StatDisplay->AddToViewport();
    }
    
	Super::BeginPlay();

    if (APlayerController* PlayerController = Cast<APlayerController>(Controller)) {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }

    if (AbilityDisplay != nullptr && AbilitySystem != nullptr)
    {
        AbilityDisplay->AddToViewport();
        AbilityDisplay->SetAbilityDisplay(AbilitySystem);
    }

    if (Crosshair != nullptr)
    {
        Crosshair->SetVisibility(ESlateVisibility::Hidden);
        Crosshair->AddToViewport();
    }
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (TargetAimOffset != CameraBoom->SocketOffset)
    {
        if (AimOffsetAlpha >= 0.95)
        {
            CameraBoom->SocketOffset = TargetAimOffset;

            if (TargetAimOffset == AimOffset && Crosshair != nullptr)
            {
                Crosshair->SetVisibility(ESlateVisibility::Visible);
            }
        }
        else
        {
            AimOffsetAlpha += 0.05;
            CameraBoom->SocketOffset = FMath::Lerp(CameraBoom->SocketOffset, TargetAimOffset, AimOffsetAlpha);
        }
    }
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::Jump);

        EnhancedInputComponent->BindAction(MainAttackAction, ETriggerEvent::Started, this, &APlayerCharacter::MainAttackInputHandler);
        EnhancedInputComponent->BindAction(MainAttackAction, ETriggerEvent::Completed, this, &APlayerCharacter::EndMainAttackInputHandler);

        EnhancedInputComponent->BindAction(AltAttackAction, ETriggerEvent::Started, this, &APlayerCharacter::AltAttackInputHandler);
        EnhancedInputComponent->BindAction(AltAttackAction, ETriggerEvent::Completed, this, &APlayerCharacter::EndMainAttackInputHandler);
        
        EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Started, this, &APlayerCharacter::BlockInputHandler);
        EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Completed, this, &APlayerCharacter::EndBlockInputHandler);

        EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Started, this, &APlayerCharacter::DodgeInputHandler);

        // EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &APlayerCharacter::Fire);

        EnhancedInputComponent->BindAction(SelectAbilityAction, ETriggerEvent::Started, this, &APlayerCharacter::UseAbility);
        EnhancedInputComponent->BindAction(SelectAbilityAction, ETriggerEvent::Completed, this, &APlayerCharacter::EndUseAbility);

        EnhancedInputComponent->BindAction(UseAbilityAction1, ETriggerEvent::Started, this, &APlayerCharacter::UseAbility1);
        EnhancedInputComponent->BindAction(UseAbilityAction1, ETriggerEvent::Completed, this, &APlayerCharacter::EndUseAbility1);

        EnhancedInputComponent->BindAction(UseAbilityAction2, ETriggerEvent::Started, this, &APlayerCharacter::UseAbility2);
        EnhancedInputComponent->BindAction(UseAbilityAction2, ETriggerEvent::Completed, this, &APlayerCharacter::EndUseAbility2);

        EnhancedInputComponent->BindAction(UseAbilityAction3, ETriggerEvent::Started, this, &APlayerCharacter::UseAbility3);
        EnhancedInputComponent->BindAction(UseAbilityAction3, ETriggerEvent::Completed, this, &APlayerCharacter::EndUseAbility3);

        EnhancedInputComponent->BindAction(LockOnAction, ETriggerEvent::Started, this, &APlayerCharacter::LockOn);

        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);

        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
    }
    else {
        UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find enhanced input component."), *GetNameSafe(this));
    }
   
}

void APlayerCharacter::Move(const FInputActionValue& Value) {
    const FVector2D MovementVector = Value.Get<FVector2D>();

    if (Controller != nullptr && !IsAttacking && !IsDodging) {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        AddMovementInput(ForwardDirection, MovementVector.Y);
        AddMovementInput(RightDirection, MovementVector.X);
    }
}

void APlayerCharacter::Look(const FInputActionValue& Value) {
    const FVector2D LookAxisVector = Value.Get<FVector2D>();

    if (Controller != nullptr) {
        AddControllerYawInput(LookAxisVector.X);
        AddControllerPitchInput(LookAxisVector.Y);
    }
}

void APlayerCharacter::MainAttackInputHandler(const FInputActionValue& Value)
{
    // SetActorRotation(FollowCamera->GetComponentRotation());
    
    MainAttack(false);
}

void APlayerCharacter::AltAttackInputHandler(const FInputActionValue& Value)
{
    // SetActorRotation(FollowCamera->GetComponentRotation());
    
    MainAttack(true);
}

void APlayerCharacter::EndMainAttackInputHandler(const FInputActionValue& Value)
{
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("attack release")));
    MainAttackRelease();
}

void APlayerCharacter::BlockInputHandler(const FInputActionValue& Value)
{
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("block press started")));
    Block();
}

void APlayerCharacter::EndBlockInputHandler(const FInputActionValue& Value)
{
    EndBlock();
}

void APlayerCharacter::DodgeInputHandler(const FInputActionValue& Value)
{
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("dodge press started")));
    Dodge(FVector::Zero());
}

void APlayerCharacter::LockOn_Implementation(const FInputActionValue& Value) {
    if (Target != nullptr) {
        Target = nullptr;
    }
    else {
        
        const FVector Start = GetActorLocation();
        const FVector End = GetActorLocation() + (FollowCamera->GetForwardVector() * LockOnTraceDistance);

        TArray<AActor*> ActorsToIgnore;

        ActorsToIgnore.Add(this);

        FHitResult HitResult;

        TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjects;

        TraceObjects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1));

        const bool Hit = UKismetSystemLibrary::SphereTraceSingleForObjects(
            GetWorld(), 
            Start, 
            End, 
            LockOnTraceRadius,
            TraceObjects,
            false,
            ActorsToIgnore,
            EDrawDebugTrace::ForDuration,
            HitResult,
            true,
            FLinearColor::Gray,
            FLinearColor::Blue);

        if (Hit) {
            Target = Cast<ABaseCharacter>(HitResult.GetActor());
        }
    }
}

void APlayerCharacter::SetCameraBoomAttachment(USceneComponent* AttachComponent) {
    const FAttachmentTransformRules Rules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::SnapToTarget, false);

    CameraBoom->AttachToComponent(AttachComponent, Rules);
}

void APlayerCharacter::UseAbility(const FInputActionValue& Value)
{
    UE_LOG(LogTemplateCharacter, Error, TEXT("Start Select Ability Value: %f"), Value.Get<float>());
    
    ActivateAbility(Value.Get<float>() - 1);
}

void APlayerCharacter::EndUseAbility(const FInputActionValue& Value) {
    UE_LOG(LogTemplateCharacter, Error, TEXT("End Select Ability Value: %f"), Value.Get<float>());

    EndAbilityActivation(Value.Get<float>() - 1);
}

void APlayerCharacter::UseAbility1(const FInputActionValue& Value)
{
    ActivateAbility(0);
}

void APlayerCharacter::EndUseAbility1(const FInputActionValue& Value)
{
    EndAbilityActivation(0);
}


void APlayerCharacter::UseAbility2(const FInputActionValue& Value)
{
    ActivateAbility(1);
}

void APlayerCharacter::EndUseAbility2(const FInputActionValue& Value)
{
    EndAbilityActivation(1);
}

void APlayerCharacter::UseAbility3(const FInputActionValue& Value)
{
    ActivateAbility(2);
}

void APlayerCharacter::EndUseAbility3(const FInputActionValue& Value)
{
    EndAbilityActivation(2);
}

void APlayerCharacter::SetAimOffset(bool ShouldAimOffset)
{
    AimOffsetAlpha = 0.0f;
    if (ShouldAimOffset)
    {
        TargetAimOffset = AimOffset;
    }
    else
    {
        TargetAimOffset = FVector::Zero();
        
        if (Crosshair != nullptr)
        {
            Crosshair->SetVisibility(ESlateVisibility::Hidden);
        }
    }
}

