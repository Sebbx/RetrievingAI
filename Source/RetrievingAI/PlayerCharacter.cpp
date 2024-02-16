#include "PlayerCharacter.h"

#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InteractionInterface.h"

#include "GameFramework/SpringArmComponent.h"

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

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
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
	FVector End = Start + ViewCamera->GetForwardVector() * 1200.f;
	
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

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &APlayerCharacter::InteractButtonPressed);
	}
}


