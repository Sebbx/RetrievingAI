// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_GIveBallToPlayer.h"
#include "Ball.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AICharacterControler.h"
#include "PlayerCharacter.h"

UBTTask_GIveBallToPlayer::UBTTask_GIveBallToPlayer(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Give Ball to Player";
}

EBTNodeResult::Type UBTTask_GIveBallToPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (auto* const AIC = Cast<AAICharacterControler>(OwnerComp.GetAIOwner()))
	{
		//AI idzie z piłką do gracza
		auto* BallToDrop = Cast<ABall>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AIC->BallToRetrieveKeyName));
		auto* BallThrower = Cast<APlayerCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AIC->BallThrowerKeyName));
		AIC->MoveToActor(BallThrower);
		float Distance = FVector::Dist(AIC->GetPawn()->GetActorLocation(), BallThrower->GetActorLocation());
		if(Distance <= AcceptanceRadius)
		{
			//Gdy gracz będzie w zasięgu, AI upuszcza piłkę i się despawn
			BallToDrop->Drop(1.f, FVector::ZeroVector);
			AIC->SetStateAsDefault();
			AIC->GetPawn()->Destroy();
			AIC->Destroy();
			BallToDrop->RemoveBallThrower();
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
		}
	} return EBTNodeResult::Failed;
}
