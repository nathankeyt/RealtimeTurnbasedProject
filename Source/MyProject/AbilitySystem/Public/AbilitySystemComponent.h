// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilitySystemComponent.generated.h"

class UAbilityMenuWidget;
class UAbility;
class UEquipAbilityBase;
class ABaseCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UAbilitySystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAbilitySystemComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<UAbility*> Abilities;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<bool> ActiveAbilities;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UAbility* MainEquippedAbility;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Instanced, meta = (AllowPrivateAccess = "true"))
	UAbilityMenuWidget* AbilityMenuWidget;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(Server, Reliable)
	void ActivateAbility(int AbilityIndex, ABaseCharacter* Character);

	UFUNCTION(Server, Reliable)
	void EndAbilityActivation(int AbilityIndex);

	void ActivateAbilityByType(UAbility* Ability, ABaseCharacter* Character);

	void EndAbilityActivationByType(UAbility* Ability);
 
	UFUNCTION(BlueprintCallable)
	TArray<UAbility*> GetAbilities() const { return Abilities; }
	
	UFUNCTION(BlueprintCallable)
	UAbility* GetMainEquippedAbility();

	UFUNCTION(BlueprintCallable)
	void AddAbility(UAbility* Ability);

	UFUNCTION(BlueprintCallable)
	void DisplayAbilityMenu();

	UFUNCTION(BlueprintCallable)
	void HideAbilityMenu();
};
