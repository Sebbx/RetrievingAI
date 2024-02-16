#include "InteractionWidget.h"
#include "Components/TextBlock.h"

void UInteractionWidget::SetHintVisibility(bool bIsVisible)
{
	if(!InteractionText) return;
	if(!bIsVisible)InteractionText->SetVisibility(ESlateVisibility::Hidden);
	else InteractionText->SetVisibility(ESlateVisibility::Visible);
}

void UInteractionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if(InteractionText) InteractionText->SetText(FText::FromString(TEXT("E")));
	SetHintVisibility(false);
}
