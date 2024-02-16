// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball.h"

#include "InteractionWidget.h"
#include "PlayerCharacter.h"
#include "PlayerHUD.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ABall::ABall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
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

// Called when the game starts or when spawned
void ABall::BeginPlay()
{
	Super::BeginPlay();
	BallMesh->OnComponentHit.AddDynamic(this, &ABall::OnComponentHit);
}

// Called every frame
void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABall::Interaction()
{
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
	//Gdy piłka w coś uderzy, można ponownie wejść z nią w interakcję
	if (!bBallThrown) return;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("Ball Landed"));
	bBallThrown = false;
	bInteractionHintBlocked = false;
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

void ABall::Throw(float ThrowStrength, FVector Direction)
{
	BallMesh->SetSimulatePhysics(true);
	BallMesh->SetPhysicsLinearVelocity(Direction * ThrowStrength);
	SetInteractHintVisibility(false);
	bInteractionHintBlocked = true;
	bBallThrown = true;
}

