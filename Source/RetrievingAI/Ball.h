#pragma once

#include "CoreMinimal.h"
#include "InteractionInterface.h"
#include "GameFramework/Actor.h"
#include "Ball.generated.h"

class UStaticMeshComponent;
class UWidgetComponent;
UCLASS()
class RETRIEVINGAI_API ABall : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* BallMesh;

	UPROPERTY(EditDefaultsOnly)
	UWidgetComponent* InteractionWidgetComponent;

	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
public:	
	ABall();
	virtual void Tick(float DeltaTime) override;
	virtual void Interaction() override;
	virtual void SetInteractHintVisibility(bool bIsVisible) override;

	void Throw(float ThrowStrength, FVector Direction);
	
protected:
	virtual void BeginPlay() override;

private:
	bool bInteractionHintBlocked = false;
	bool bBallThrown = false;
};
