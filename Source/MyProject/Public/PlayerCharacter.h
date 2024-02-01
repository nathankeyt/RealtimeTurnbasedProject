// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "PlayerCharacter.generated.h"

class UWidget;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UAbilityDisplayWidget;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS()
class MYPROJECT_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(EditAnywhere, Category = Camera)
	FVector AimOffset;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	FVector TargetAimOffset;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	float AimOffsetAlpha = 1.0f;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* JumpAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* SelectAbilityAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* UseAbilityAction1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* UseAbilityAction2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* UseAbilityAction3;
    
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* MainAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* AltAttackAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* LockOnAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* BlockAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* DodgeAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* FireAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, Category = UI)
	UAbilityDisplayWidget* AbilityDisplay;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, Category = UI)
	UUserWidget* Crosshair;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trace")
	float LockOnTraceRadius = 500.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trace")
	float LockOnTraceDistance = 500.0f;

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);
	
	void MainAttackInputHandler(const FInputActionValue& Value);

	void EndMainAttackInputHandler(const FInputActionValue& Value);
	
	void BlockInputHandler(const FInputActionValue& Value);

	void EndBlockInputHandler(const FInputActionValue& Value);
	
	void DodgeInputHandler(const FInputActionValue& Value);
	
	void AltAttackInputHandler(const FInputActionValue& Value);

	UFUNCTION(Server, Reliable)
	void LockOn(const FInputActionValue& Value);

	void UseAbility(const FInputActionValue& Value);

	void EndUseAbility(const FInputActionValue& Value);

	void UseAbility1(const FInputActionValue& Value);

	void EndUseAbility1(const FInputActionValue& Value);

	void UseAbility2(const FInputActionValue& Value);

	void EndUseAbility2(const FInputActionValue& Value);

	void UseAbility3(const FInputActionValue& Value);

	void EndUseAbility3(const FInputActionValue& Value);

	// void Fire(const FInputActionValue& Value);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	void SetAimOffset(bool ShouldAimOffset);
	
	UFUNCTION(BlueprintCallable, Category="PlayerCamera")
	void SetCameraBoomAttachment(USceneComponent* AttachComponent);
	
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};