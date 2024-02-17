// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_ChaseBall.generated.h"

/**
 * 
 */
UCLASS()
class RETRIEVINGAI_API UBTTask_ChaseBall : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	explicit UBTTask_ChaseBall(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8 *NodeMemory) override;

	UPROPERTY(EditAnywhere)
	float AcceptanceRadius = 100.f;
};
