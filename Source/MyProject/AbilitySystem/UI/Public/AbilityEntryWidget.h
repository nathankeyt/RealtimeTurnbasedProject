
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "AbilityEntryWidget.generated.h"

class UAbility;
class UTextBlock;
class UButton;
/**
 * 
 */
UCLASS()
class MYPROJECT_API UAbilityEntryWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UButton* SelectAbilityButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* AbilityName;

public:
	UFUNCTION(BlueprintCallable)
	void SetAbility(UAbility* Ability);
};


