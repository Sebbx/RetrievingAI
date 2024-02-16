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

UCLASS()
class RETRIEVINGAI_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* ViewCamera;

	IInteractionInterface* LastHittedInteractableActor = nullptr;

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

protected:
	virtual void BeginPlay() override;

	//Input
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enchanced Input")
	class UInputMappingContext* InputMappingContext;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enchanced Input")
	class UInputAction* JumpAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enchanced Input")
	UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enchanced Input")
	UInputAction* LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enchanced Input")
	UInputAction* InteractAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enchanced Input")
	UInputAction* ThrowAction;

private:
	UPROPERTY(EditDefaultsOnly, Category = Interaction)
	float InteractRange = 1200.f;

	UPROPERTY(EditDefaultsOnly, Category = ThrowingBall)
	float ThrowStrength = 1000.f;
	
	void ManageLineTrace();

	UPROPERTY(EditAnywhere, Category = ThrowingBall)
	int32 StopIgnoringBallRemainingCalls = 1;
	int32 CallTracker;
	FTimerHandle TimerHandle;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> PlayerHudClass;
	
	
	void StopIgnoringBallTimer();
	
};
