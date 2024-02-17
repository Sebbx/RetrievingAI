#include "AICharacter.h"
#include "AICharacterControler.h"

#include "BehaviorTree/BehaviorTree.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Components/CapsuleComponent.h"

AAICharacter::AAICharacter()
{
 	PrimaryActorTick.bCanEverTick = true;
}

void AAICharacter::SetAICValues(ABall* BallToRetrieve, UBehaviorTree* BehaviorTree,  APlayerCharacter* BallThrower)
{
	if(!BallToRetrieve || !BehaviorTree) return;
	Cast<AAICharacterControler>(UAIBlueprintHelperLibrary::GetAIController(this))->InitializeAIC(BallToRetrieve, BehaviorTree, BallThrower);
}

void AAICharacter::BeginPlay()
{
	Super::BeginPlay();
	GetMesh()->SetCollisionProfileName("IgnoreBall");
	GetCapsuleComponent()->SetCollisionProfileName("IgnoreBall");
}

void AAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

