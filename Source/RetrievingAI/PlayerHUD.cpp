// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"
#include "Components/TextBlock.h"

void UPlayerHUD::UpdateHUD(bool bIsBallInHand)
{
	if (!IsBallInHandTextBlock) return;

	if(bIsBallInHand)
	{
		IsBallInHandTextBlock->SetText(FText::FromString("Ball in hand!"));
		IsBallInHandTextBlock->SetColorAndOpacity(FLinearColor(FColor::Green));
	}
	else
	{
		IsBallInHandTextBlock->SetText(FText::FromString("No ball in hand"));
		IsBallInHandTextBlock->SetColorAndOpacity(FLinearColor(FColor::Red));
	}
}

void UPlayerHUD::NativeConstruct()
{
	Super::NativeConstruct();
	UpdateHUD(false);
	
}
