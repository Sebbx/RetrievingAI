#include "PlayerCharacter.h"

#include "Ball.h"
#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InteractionInterface.h"
#include "PlayerHUD.h"
#include "Components/CapsuleComponent.h"

#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->TargetArmLength = 500.f;
	SpringArmComponent->bUsePawnControlRotation = true;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	ViewCamera->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	PlayerHUD = nullptr;

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}

	if(!PlayerHudClass) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Player HUD Class is propably not set in BP_PlayerCharacter"));
	else
	{
		PlayerHUD = CreateWidget<UPlayerHUD>(GetWorld(), PlayerHudClass);
		PlayerHUD->AddToPlayerScreen();
	}
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ManageLineTrace();
}

void APlayerCharacter::ManageLineTrace()
{
	FVector Start = ViewCamera->GetComponentLocation();
	FVector End = Start + ViewCamera->GetForwardVector() * InteractRange;
	
	FHitResult OutHit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	
	if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, Params))
	{
		//Debugowa sfera aby ułatwić celowanie
		DrawDebugSphere(GetWorld(), OutHit.Location, 8.f, 8, FColor::Red, false, 0.f, 0, 1.f);
		
		//Wyświetlenie indykatora interakcji po trafieniu tracem aktora, który implementuje interfejs
		if (OutHit.Actor->Implements<UInteractionInterface>())
		{
			LastHittedInteractableActor = Cast<IInteractionInterface>(OutHit.Actor);
			LastHittedInteractableActor->SetInteractHintVisibility(true);
		}
		/*Jeżeli trace już nie trafia w pożądanego aktora, a w pamięci jest zmienna LastHittedInteractableActor,
		 *oznacza to że chwilę wcześniej użytkownik mierzył w skrzynię, ale już tego nie robi,
		 *więc można wyłączyć indykator interakcji temu aktorowi oraz ustawić zmienną na nullptr*/
		else if (LastHittedInteractableActor)
		{
			LastHittedInteractableActor->SetInteractHintVisibility(false);
			LastHittedInteractableActor = nullptr;
		}
	}
}

void APlayerCharacter::StopIgnoringBallTimer()
{
	CallTracker--;
	if (CallTracker <=0)
	{
		GetMesh()->SetCollisionProfileName("CharacterMesh");
		GetCapsuleComponent()->SetCollisionProfileName("CharacterMesh");
		GetWorldTimerManager().ClearTimer(TimerHandle);
	}
}


void APlayerCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	if (Controller && (MovementVector.X != 0 || MovementVector.Y != 0))
	{
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(ForwardDirection, MovementVector.Y);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector =  Value.Get<FVector2D>();
	
	if (Controller && (LookAxisVector.X != 0 || LookAxisVector.Y != 0))
	{
		AddControllerPitchInput(LookAxisVector.Y);
		AddControllerYawInput(LookAxisVector.X);
	}
}

void APlayerCharacter::InteractButtonPressed(const FInputActionValue& Value)
{
	if (LastHittedInteractableActor)
	{
		LastHittedInteractableActor->Interaction();
	}
}

void APlayerCharacter::ThrowButtonPressed(const FInputActionValue& Value)
{
	if (!BallInHand) return;
	//Ustawiam profil kolizji na ignorowanie piłki, aby przy rzucie nie odbiła się od gracza
	GetMesh()->SetCollisionProfileName("IgnoreBall");
	GetCapsuleComponent()->SetCollisionProfileName("IgnoreBall");
	
	//Gdy timer dobiegnie końca, character będzie ponownie blokować piłki
	CallTracker = StopIgnoringBallRemainingCalls;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &APlayerCharacter::StopIgnoringBallTimer, 0.3f, true, 0.f);

	//Ustawiam kierunek rzutu względem LookAtRotation z hand socketu a forward vectorem kamery
	FVector Start = GetMesh()->GetSocketLocation("HandSocket");
	FVector End = ViewCamera->GetForwardVector() * 9999;
	
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(BallInHand);
	FVector Direction = UKismetMathLibrary::FindLookAtRotation(Start, End).Vector();
	BallInHand->Throw(ThrowStrength, Direction);
	PlayerHUD->UpdateHUD(false);
	BallInHand = nullptr;
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &APlayerCharacter::InteractButtonPressed);
		EnhancedInputComponent->BindAction(ThrowAction, ETriggerEvent::Triggered, this, &APlayerCharacter::ThrowButtonPressed);
	}
}


