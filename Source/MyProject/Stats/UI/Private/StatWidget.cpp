// Fill out your copyright notice in the Description page of Project Settings.


#include "MyProject/Stats/UI/Public/StatWidget.h"

#include "Components/ProgressBar.h"

void UStatWidget::DisplayHealthPercentage(const float CurrHealth, const float MaxHealth) const
{
	HealthBar->SetPercent(CurrHealth / MaxHealth);
}

void UStatWidget::DisplayStaminaPercentage(const float CurrStamina, const float MaxStamina) const
{
	StaminaBar->SetPercent(CurrStamina / MaxStamina);
}
