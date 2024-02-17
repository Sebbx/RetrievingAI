// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Ball.h"
#include "AICharacterControler.generated.h"

class UBehaviorTree;
class AAICharacter;

UENUM(BlueprintType)
enum class EAIState : uint8
{
	EAS_Default,
	EAS_ChaseBall,
	EAS_GiveBallToPlayer
};

UCLASS()
class RETRIEVINGAI_API AAICharacterControler : public AAIController
{
	GENERATED_BODY()
public:
	ABall* BallToRetrieve;

	void InitializeAIC(ABall* Ball, UBehaviorTree* Tree, APlayerCharacter* BallThrower);
	void SetStateAsChaseBall();
	void SetStateAsCollectBall();
	void SetStateAsGiveBallToPlayer();
	void SetStateAsDropBall();
	void SetStateAsDefault();

	FName AIStateKeyName = "AIState";
	FName BallToRetrieveKeyName = "BallToRetrieve";
	FName BallThrowerKeyName ="BallThrower";
	
protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	
	UBlackboardComponent* Blackboard;
	AAICharacter* AICharacterRef;

	
};
