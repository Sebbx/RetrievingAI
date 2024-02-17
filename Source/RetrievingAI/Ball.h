#pragma once

#include "CoreMinimal.h"
#include "InteractionInterface.h"
#include "GameFramework/Actor.h"
#include "Ball.generated.h"

class AAICharacter;
class APlayerCharacter;
class UStaticMeshComponent;
class UWidgetComponent;
UCLASS()
class RETRIEVINGAI_API ABall : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	

	UPROPERTY(EditDefaultsOnly)
	UWidgetComponent* InteractionWidgetComponent;

	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
public:	
	ABall();
	virtual void Tick(float DeltaTime) override;
	virtual void Interaction() override;
	virtual void SetInteractHintVisibility(bool bIsVisible) override;
	
	bool bInteractionHintBlocked = false;
	bool bBallThrown = false;
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* BallMesh;

	void Throw(float ThrowStrength, FVector Direction, APlayerCharacter* Thrower, FVector AISpawnLocation);
	void Drop(float DropStrength, FVector Direction);
	void SpawnAI();

	void RemoveBallThrower();
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "AI")
	TSubclassOf<AAICharacter> AICharacterClass;
	
	APlayerCharacter* BallThrower;
	FVector AISpawnVector;
};
