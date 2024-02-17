#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AICharacter.generated.h"


class UBehaviorTree;
class AAICharacterControler;
UCLASS()
class RETRIEVINGAI_API AAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AAICharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetAICValues(class ABall* BallToRetrieve, UBehaviorTree* BehaviorTree, class APlayerCharacter* BallThrower);

protected:
	virtual void BeginPlay() override;

private:
	
	AAICharacterControler* AICharacterControler;
};
