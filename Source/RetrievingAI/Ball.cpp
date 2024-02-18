#include "Ball.h"
#include "AICharacter.h"
#include "InteractionWidget.h"
#include "PlayerCharacter.h"
#include "PlayerHUD.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

ABall::ABall()
{
	PrimaryActorTick.bCanEverTick = true;
	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>("BallMesh");
	RootComponent = BallMesh;

	BallMesh->SetCollisionProfileName("Ball");
	BallMesh->SetSimulatePhysics(true);

	InteractionWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidget"));
	InteractionWidgetComponent->SetupAttachment(RootComponent);
	InteractionWidgetComponent->SetDrawAtDesiredSize(true);
	InteractionWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);

}

void ABall::BeginPlay()
{
	Super::BeginPlay();
	BallMesh->OnComponentHit.AddDynamic(this, &ABall::OnComponentHit);
}

void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABall::Interaction()
{
	//Gdy interaction hint nie jest zablokowany, wyłączam fizykę na BallMeshu a następnie attachuje piłkę do sockecie w prawej ręce mesha gracza
	if (bInteractionHintBlocked) return;
	IInteractionInterface::Interaction();
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	BallMesh->SetSimulatePhysics(false);
	AttachToComponent(PlayerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "HandSocket");
	PlayerCharacter->BallInHand = this;
	PlayerCharacter->PlayerHUD->UpdateHUD(true);
	SetInteractHintVisibility(false);
}

void ABall::OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (!bBallThrown) return;
	//Gdy piłka w coś uderzy, można ponownie wejść z nią w interakcję
	bBallThrown = false;
	bInteractionHintBlocked = false;

	if(BallThrower) SpawnAI();
	
	
}
void ABall::SetInteractHintVisibility(bool bIsVisible)
{
	//Ustawienie widoczności indykatora interakcji
	if (!bInteractionHintBlocked && !bBallThrown)
	{
		IInteractionInterface::SetInteractHintVisibility(bIsVisible);
		if(InteractionWidgetComponent) Cast<UInteractionWidget>(InteractionWidgetComponent->GetWidget())->SetHintVisibility(bIsVisible);
	}
}

void ABall::Throw(float ThrowStrength, FVector Direction, APlayerCharacter* Thrower, FVector AISpawnLocation)
{
	//Wyłączenie fizyki, aby piłka odczepiła się od socketa oraz by oczywiście działała fizyka
	BallMesh->SetSimulatePhysics(true);
	BallMesh->SetPhysicsLinearVelocity(Direction * ThrowStrength);
	SetInteractHintVisibility(false);
	bInteractionHintBlocked = true;
	bBallThrown = true;

	BallThrower = Thrower;
	AISpawnVector = AISpawnLocation;
}

void ABall::Drop(float DropStrength, FVector Direction)
{
	BallMesh->SetSimulatePhysics(true);
	BallMesh->SetPhysicsLinearVelocity(Direction * DropStrength);
	SetInteractHintVisibility(false);
	bInteractionHintBlocked = true;
	bBallThrown = true;
}

void ABall::SpawnAI()
{
	if(!AICharacterClass) return;
	AAICharacter* AIRetriever = Cast<AAICharacter>(UAIBlueprintHelperLibrary::SpawnAIFromClass(
		GetWorld(),
		AICharacterClass,
		BallThrower->BehaviorTree,
		AISpawnVector,
		FRotator::ZeroRotator,
		false,
		BallThrower));
	AIRetriever->SetAICValues(this, BallThrower->BehaviorTree, BallThrower);
	bInteractionHintBlocked = true;
}

void ABall::RemoveBallThrower()
{
	BallThrower = nullptr;
}

