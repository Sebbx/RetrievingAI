// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class RETRIEVINGAI_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateHUD(bool bIsBallInHand);

protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	class UTextBlock* IsBallInHandTextBlock;
	
};
