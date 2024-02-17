// Fill out your copyright notice in the Description page of Project Settings.

#include "AICharacterControler.h"
#include "AICharacter.h"
#include "PlayerCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

void AAICharacterControler::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	AICharacterRef = Cast<AAICharacter>(GetPawn());
}

void AAICharacterControler::InitializeAIC(ABall* Ball, UBehaviorTree* Tree, APlayerCharacter* BallThrower)
{
	//Przypisanie wartości oraz ustawienie stanu na ChaseBall, ponieważ gdy spawni się AI to piłka już uderzyła w ziemię, i AI może zacząć aportować
	UseBlackboard(Tree->GetBlackboardAsset(), Blackboard);
	RunBehaviorTree(Tree);
	BallToRetrieve = Ball;
	Blackboard->SetValueAsObject(BallToRetrieveKeyName, BallToRetrieve);
	Blackboard->SetValueAsObject(BallThrowerKeyName, BallThrower);
	SetStateAsChaseBall();
}

void AAICharacterControler::SetStateAsChaseBall()
{
	Blackboard->SetValueAsEnum(AIStateKeyName, (uint8)EAIState::EAS_ChaseBall);
}

void AAICharacterControler::SetStateAsGiveBallToPlayer()
{
	Blackboard->SetValueAsEnum(AIStateKeyName, (uint8)EAIState::EAS_GiveBallToPlayer);
}

void AAICharacterControler::SetStateAsDefault()
{
	Blackboard->SetValueAsEnum(AIStateKeyName, (uint8)EAIState::EAS_Default);
}
