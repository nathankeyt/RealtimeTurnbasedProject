// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
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

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

    if (APlayerController* PlayerController = Cast<APlayerController>(Controller)) {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::Jump);

        EnhancedInputComponent->BindAction(MainAttackAction, ETriggerEvent::Started, this, &APlayerCharacter::MainAttackInputHandler);
        
        EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Started, this, &APlayerCharacter::BlockInputHandler);
        EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Completed, this, &APlayerCharacter::EndBlockInputHandler);
        
        EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &APlayerCharacter::Fire);

        EnhancedInputComponent->BindAction(SelectAbilityAction, ETriggerEvent::Started, this, &APlayerCharacter::UseAbility);

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

    if (Controller != nullptr && ! IsAttacking) {
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
    
    MainAttack();
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
            Target = HitResult.GetActor();
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Hit %s"), *Target->GetName()));
        }
    }
}

void APlayerCharacter::SetCameraBoomAttachment(USceneComponent* AttachComponent) {
    const FAttachmentTransformRules Rules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::SnapToTarget, false);

    CameraBoom->AttachToComponent(AttachComponent, Rules);
}

void APlayerCharacter::UseAbility(const FInputActionValue& Value)
{
    UE_LOG(LogTemplateCharacter, Error, TEXT("'%f' Failed to find enhanced input component."), Value.Get<float>());
    
    ActivateAbility(Value.Get<float>() - 1);
}

void APlayerCharacter::Fire(const FInputActionValue& Value)
{
    
}
