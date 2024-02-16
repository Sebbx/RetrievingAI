// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionWidget.generated.h"

/**
 * 
 */
UCLASS()
class RETRIEVINGAI_API UInteractionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetHintVisibility(bool bIsVisible);
	
protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	class UTextBlock* InteractionText;
	
};
