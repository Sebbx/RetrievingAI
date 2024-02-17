#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class IInteractionInterface;
class ABall;
class UPlayerHUD;
class UNiagaraComponent;
class UInputMappingContext;
class UInputAction;
class UBehaviorTree;

UCLASS()
class RETRIEVINGAI_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	USpringArmComponent* SpringArmComponent;
	
	UPROPERTY(EditDefaultsOnly)
	UCameraComponent* ViewCamera;
	
	UPROPERTY(EditDefaultsOnly)
	UNiagaraComponent* NiagaraComponent;
	
	IInteractionInterface* LastHitInteractableActor = nullptr;

public:
	APlayerCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void InteractButtonPressed(const FInputActionValue& Value);
	void ThrowButtonPressed(const FInputActionValue& Value);

	ABall* BallInHand;
	UPlayerHUD* PlayerHUD;
	
	UPROPERTY(EditAnywhere, Category = AIRetriever)
	UBehaviorTree* BehaviorTree;
	
	//Input
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enchanced Input")
	UInputMappingContext* InputMappingContext;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enchanced Input")
	UInputAction* JumpAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enchanced Input")
	UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enchanced Input")
	UInputAction* LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enchanced Input")
	UInputAction* InteractAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enchanced Input")
	UInputAction* ThrowAction;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = Interaction)
	float InteractRange = 1200.f;
	
	UPROPERTY(EditDefaultsOnly, Category = ThrowingBall)
	float ThrowStrength = 1000.f;
	
	UPROPERTY(EditAnywhere, Category = ThrowingBall)
	int32 StopIgnoringBallRemainingCalls = 1;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> PlayerHudClass;
	
	FVector ThrowEnd;
	int32 CallTracker;
	FTimerHandle TimerHandle;
	
	void ManageThrowTrajectory();
	void ManageLineTrace();
	void StopIgnoringBallTimer();
	
	FVector AISpawnLocation = FVector::ZeroVector;
	
};
