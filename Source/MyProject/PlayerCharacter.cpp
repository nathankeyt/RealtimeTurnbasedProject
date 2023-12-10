// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetArrayLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilityDisplayWidget.h"

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

    if (PunchStart && PunchIKAlpha > 0.01f) {
        PunchStart = false;
    }
    
    if (!CanPunch) {
        CheckFistCollision(PunchBoneName);
    }

    if (IncreasePunchIKAlpha && PunchIKAlpha < 1.0f) {
        PunchIKAlpha += 0.10f;
    }
    else if (!IncreasePunchIKAlpha && PunchIKAlpha > 0.01f) {
        PunchIKAlpha -= 0.10f;
    }

    /*
    if (IsPunching) {
        CheckFistCollision();
    } */
    /*
    if (Controller != nullptr && LockOnTarget != nullptr) {
        Controller->SetControlRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), LockOnTarget->GetActorLocation()));
    }*/

    if (GetMesh() != nullptr && LockOnTarget != nullptr) {
        SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), LockOnTarget->GetActorLocation()));
        // GEngine->AddOnScreenDebugMessage(-1, 60.0f, FColor::Blue, FString::Printf(TEXT("Forward: %s"), *GetActorForwardVector().ToString()));
    }
}

void APlayerCharacter::CheckFistCollision(FName BoneName) {
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
        CurrPunchHits,
        EDrawDebugTrace::ForDuration,
        HitResult,
        true);

    if (Hit) {
        AActor* HitActor = HitResult.GetActor();
        ABaseCharacter* HitEnemy = Cast<ABaseCharacter>(HitActor);

        CurrPunchHits.Add(HitActor);

        if (HitEnemy != nullptr) {
            HitEnemy->PlayHitReaction();
        }
        
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Hit %s"), *HitActor->GetName()));
    }
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::Jump);

        EnhancedInputComponent->BindAction(PunchAction, ETriggerEvent::Started, this, &APlayerCharacter::Punch);
        
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
    FVector2D MovementVector = Value.Get<FVector2D>();

    if (Controller != nullptr && !IsPunching) {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        AddMovementInput(ForwardDirection, MovementVector.Y);
        AddMovementInput(RightDirection, MovementVector.X);
    }
}

void APlayerCharacter::Look(const FInputActionValue& Value) {
    FVector2D LookAxisVector = Value.Get<FVector2D>();

    if (Controller != nullptr) {
        AddControllerYawInput(LookAxisVector.X);
        AddControllerPitchInput(LookAxisVector.Y);
    }
}

void APlayerCharacter::Punch(const FInputActionValue& Value) {
    if (Controller != nullptr && CanPunch) {
        CanPunch = false;
        IsPunching = true;
        

        const FVector Start = GetActorLocation() + (FollowCamera->GetForwardVector() * 50);
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
            FVector(TraceRadius), 
            Controller->GetControlRotation(), 
            TraceObjects, 
            false, 
            ActorsToIgnore, 
            EDrawDebugTrace::ForDuration,
            HitResult,
            true);

        if (Hit) {
            LockOnTarget = HitResult.GetActor();
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Hit %s"), *LockOnTarget->GetName()));
        }

        ACharacter::PlayAnimMontage(CombatMontages[FMath::RandRange(0, CombatMontages.Num() - 1)]);

        FOnMontageEnded EndDelegate;
        
        EndDelegate.BindUObject(this, &APlayerCharacter::OnPunchingMontageEnd);
        
        GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(EndDelegate);

        //GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &AMyCharacter::OnPunchingMontageEnd);
    }  
}

void APlayerCharacter::LockOn(const FInputActionValue& Value) {
    if (LockOnTarget != nullptr) {
        LockOnTarget = nullptr;
    }
    else {
        const FVector Start = GetActorLocation();
        const FVector End = GetActorLocation() + (FollowCamera->GetForwardVector() * TraceDistance);

        TArray<AActor*> ActorsToIgnore;

        ActorsToIgnore.Add(this);

        FHitResult HitResult;

        TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjects;

        TraceObjects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1));

        const bool Hit = UKismetSystemLibrary::SphereTraceSingleForObjects(
            GetWorld(), 
            Start, 
            End, 
            TraceRadius,
            TraceObjects,
            false,
            ActorsToIgnore,
            EDrawDebugTrace::ForDuration,
            HitResult,
            true,
            FLinearColor::Gray,
            FLinearColor::Blue);

        if (Hit) {
            LockOnTarget = HitResult.GetActor();
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Hit %s"), *LockOnTarget->GetName()));
        }
    }
}

void APlayerCharacter::SetCameraBoomAttachment(USceneComponent* AttachComponent) {
    FAttachmentTransformRules rules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::SnapToTarget, false);

    CameraBoom->AttachToComponent(AttachComponent, rules);
}

void APlayerCharacter::UseAbility(const FInputActionValue& Value)
{
    UE_LOG(LogTemplateCharacter, Error, TEXT("'%f' Failed to find enhanced input component."), Value.Get<float>());
    
    AbilitySystem->ActivateAbility(Value.Get<float>() - 1, this);
}


void APlayerCharacter::Fire() {
    if (ProjectileClass) {
        FVector CharacterLocation = GetMesh()->GetBoneLocation("index_03_l");

        FVector MuzzleOffset;
        MuzzleOffset.Set(100.0f, 0.0f, 0.0f);

        FVector MuzzleLocation = CharacterLocation + FTransform(FollowCamera->GetComponentRotation()).TransformVector(MuzzleOffset); // try muzzleroation.vector() * muzzleoffset

        const FVector Start = FollowCamera->GetComponentLocation();
        const FVector End = Start + (FollowCamera->GetForwardVector() * 9000.0f);

        TArray<AActor*> ActorsToIgnore;
        ActorsToIgnore.Add(this);

        FHitResult HitResult;

        /*
        const bool Hit = GetWorld()->LineTraceSingleByChannel(
            HitResult,
            Start,
            End,
            ECollisionChannel::ECC_Visibility,
            QueryParams);
        */

        const bool Hit = UKismetSystemLibrary::LineTraceSingle(
            GetWorld(),
            Start,
            End,
            UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility),
            false,
            ActorsToIgnore,
            EDrawDebugTrace::None,
            HitResult,
            true);
		

        FRotator MuzzleRotation;

        if (Hit) {
            MuzzleRotation = UKismetMathLibrary::FindLookAtRotation(MuzzleLocation, HitResult.Location);
        }
        else {
            MuzzleRotation = UKismetMathLibrary::FindLookAtRotation(MuzzleLocation, End);
        }

        UWorld* World = GetWorld();

        if (World) {
            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = this;
            SpawnParams.Instigator = GetInstigator();

            AProjectile* Projectile = World->SpawnActor<AProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			
            if (Projectile) {
                ACharacter::PlayAnimMontage(ShootMontage);

                FVector LaunchDirection = MuzzleRotation.Vector();

                Projectile->FireInDirection(LaunchDirection);

                /*
                if (PlayerHUD) {
                    PlayerHUD->SetAmmo(--Ammo);
                }	*/
            }
        }
    }
}