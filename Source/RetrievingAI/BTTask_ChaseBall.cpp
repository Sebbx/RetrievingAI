// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_ChaseBall.h"
#include "AICharacter.h"
#include "Ball.h"
#include "AICharacterControler.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_ChaseBall::UBTTask_ChaseBall(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Chase Ball";
}

EBTNodeResult::Type UBTTask_ChaseBall::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (auto* const AIC = Cast<AAICharacterControler>(OwnerComp.GetAIOwner()))
	{
		//AI będzie podążać za piłką
		auto* BallToRetrieve = Cast<ABall>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AIC->BallToRetrieveKeyName));
		AIC->MoveToActor(BallToRetrieve);
		float Distance = FVector::Dist(AIC->GetPawn()->GetActorLocation(), BallToRetrieve->GetActorLocation());
		if(Distance <= AcceptanceRadius)
		{
			//Gdy piłka będzie w zasięgu, zostaje zaatachowana do dłoni AI i state zostaje zmieniony na GiveBallToPlayer
			BallToRetrieve->BallMesh->SetSimulatePhysics(false);
			BallToRetrieve->AttachToComponent(Cast<AAICharacter>(AIC->GetPawn())->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "HandSocket");
			AIC->SetStateAsGiveBallToPlayer();
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
		}
	} return EBTNodeResult::Failed;
	
}
