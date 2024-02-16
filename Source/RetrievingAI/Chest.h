#pragma once

#include "CoreMinimal.h"
#include "InteractionInterface.h"
#include "GameFramework/Actor.h"
#include "Chest.generated.h"

class ABall;
class UStaticMeshComponent;
class UWidgetComponent;
class ABall;
	
enum class EChestState : uint8
{
	ECS_Open,
	ECS_Closed,
	ECS_Opening,
	ECS_Closing
};

UCLASS()
class RETRIEVINGAI_API AChest : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, Category = ChestElements)
	UStaticMeshComponent* ContainerMesh;

	UPROPERTY(VisibleAnywhere, Category = ChestElements)
	UStaticMeshComponent* LidMesh;

	UPROPERTY(VisibleAnywhere, Category = ChestElements)
	USceneComponent* LidPivot;
	
	UPROPERTY(EditDefaultsOnly)
	UWidgetComponent* InteractionWidgetComponent;
	
	
public:	
	AChest();
	virtual void Tick(float DeltaTime) override;

	virtual void Interaction() override;
	virtual void SetInteractHintVisibility(bool bIsVisible) override;
	

protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere, Category = OpeningSystem)
	float OpenRotationOffset = 90.f;

	UPROPERTY(EditAnywhere, Category = OpeningSystem)
	float OpenRotationSpeed = 300.f;

	EChestState CurrentChestState = EChestState::ECS_Closed;

	float TargetRotation;
	
	bool bTestBoolRotation = false;
	bool bTargetValuesSet = false;
	bool bInteractionHintBlocked = false;
	float RotateDirection;
	
	void ManagePivotRotation(float DeltaTime);
	void DropBall();

	UPROPERTY(EditAnywhere, Category = BallDrop)
	int32 BallDropDelayTimerRemainingCalls = 2;
	UPROPERTY(EditAnywhere, Category = BallDrop)
	int32 CloseLidDelayTimerRemainingCalls = 4;
	int32 CallTracker;
	FTimerHandle TimerHandle;

	void BallDropDelayTimer();
	void CloseLidDelayTimer();

	UPROPERTY(EditDefaultsOnly, Category = BallDrop)
	TSubclassOf<ABall> BallBPClass;

	UPROPERTY(EditDefaultsOnly, Category = BallDrop)
	float BallDropStrength = 1000.f;
	
	UPROPERTY(EditDefaultsOnly, Category = BallDrop)
	FVector BallDropDirection;

	ABall* DroppedBall;
};
