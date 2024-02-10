// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyProject/Characters/Public/BaseCharacter.h"

#include "NPCCharacter.generated.h"

class APlayerCharacter;
class UWidgetComponent;

UCLASS()
class MYPROJECT_API ANPCCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPCCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ShouldScanForPlayer = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ScanRadius = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UWidgetComponent* WidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	APlayerCharacter* PlayerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* SelectedMaterial;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ScanForPlayer();

	void SetShouldScanForPlayer(bool ShouldScan);

	virtual void CleanupOnDeath() override;
};
