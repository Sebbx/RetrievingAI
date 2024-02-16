#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ball.generated.h"

UCLASS()
class RETRIEVINGAI_API ABall : public AActor
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* BallMesh;
	
public:	
	ABall();
	virtual void Tick(float DeltaTime) override;

	void Throw(float ThrowStrength, FVector Direction);

	bool bIsThrown = false;

protected:
	virtual void BeginPlay() override;

private:


};
