#include "Chest.h"

#include "Ball.h"
#include "InteractionWidget.h"
#include "Components/WidgetComponent.h"

AChest::AChest()
{
	PrimaryActorTick.bCanEverTick = true;
	
	ContainerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ContainerMesh"));
	RootComponent = ContainerMesh;
	
	LidPivot = CreateDefaultSubobject<USceneComponent>(TEXT("LidPivot"));
	LidPivot->SetupAttachment(RootComponent);
	
	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMesh"));
	LidMesh->SetupAttachment(LidPivot);

	InteractionWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidget"));
	InteractionWidgetComponent->SetupAttachment(RootComponent);
}

void AChest::BeginPlay()
{
	Super::BeginPlay();
	
}

void AChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ManagePivotRotation(DeltaTime);
}

void AChest::ManagePivotRotation(float DeltaTime)
{
	if(bTestBoolRotation)
	{
		if(!bTargetValuesSet)
		{
			//Zależnie od obecnego stanu skrzyni, ustawia się docelowy kierunek rotacji pivota
			if (CurrentChestState == EChestState::ECS_Closed)
			{
				RotateDirection = 1;
				CurrentChestState = EChestState::ECS_Opening;
			}
			else if(CurrentChestState == EChestState::ECS_Open)
			{
				RotateDirection = -1;
				CurrentChestState = EChestState::ECS_Closing;
			}

			//Docelowy pitch to wybrany offset otwarcia pomnożony przez kierunek rotacji
			TargetRotation = (LidPivot->GetRelativeRotation().Pitch + (OpenRotationOffset * RotateDirection));
			bTargetValuesSet = true;
		}
		else
		{
			//Ustawienie relatywnej rotacji za pomocną funkcji interpolującej z aktualnego pitcha pivota do docelowej rotacji
			LidPivot->SetRelativeRotation(FRotator(FMath::FInterpConstantTo(LidPivot->GetRelativeRotation().Pitch, TargetRotation, DeltaTime, OpenRotationSpeed), 0, 0));

			//Porównanie obecnej rotacji pivota z docelową z tolerancją błędu aby zapobiec ewentualnemu blokerowi z zacięciem się systemu otwierania
			if(FMath::IsNearlyEqual(TargetRotation, LidPivot->GetRelativeRotation().Pitch, 2.f))	
			{
				//Ustawienie rotacji pivota do docelowej aby usunąć ewentualne rozbieżności, które mogły wyniknąć przy samym otwieraniu, nieperfekcyjnej dokładności floata oraz oczywiście przy IsNearlyEqual
				LidPivot->SetRelativeRotation(FRotator(TargetRotation, 0, 0));

				//Gdy skrzynia się zamknie może ponownie być otwarta
				if (CurrentChestState == EChestState::ECS_Closing)
				{
					CurrentChestState = EChestState::ECS_Closed;
					bTestBoolRotation = false;
					bInteractionHintBlocked = false;
				}
				// Gdy skrzynia się otworzy, startuje timer, który opóźnia wyrzucenie piłki - zamiar czysto wizualny
				else if (CurrentChestState == EChestState::ECS_Opening)
				{
					CallTracker = BallDropDelayTimerRemainingCalls;
					GetWorldTimerManager().SetTimer(TimerHandle, this, &AChest::BallDropDelayTimer, 0.3f, true, 0.f);
					CurrentChestState = EChestState::ECS_Open;
					bTestBoolRotation = false;
				}
				bTargetValuesSet = false;
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AA"));
			}
		}
	}
}

void AChest::DropBall()
{
	//Kolizje skrzyni dla piłek zostają tymczasowo wyłączone aby piłka mogła swobodnie wylecieć ze skrzyni
	ContainerMesh->SetCollisionProfileName("IgnoreBall");
	LidMesh->SetCollisionProfileName("IgnoreBall");
	DroppedBall = GetWorld()->SpawnActor<ABall>(BallBPClass, ContainerMesh->GetCenterOfMass(), FRotator::ZeroRotator);

	//Uzyskuję tutaj wektor zwrócony do góry i delikatnie w przód poprzez 4-krotne podzielenie sumy dwóch wektorów
	//Podzielenie przez 2 sumy dwóch wektorów skutkuje uzyskaniem wektorem między sumowanymi wektorami, więc doszedłem do wniosku,
	//że podzielenie przez większą liczbę da mi pożądany kąt wyrzutu piłki skierowany bardziej ku górze
	DroppedBall->Throw(BallDropStrength, this->GetActorUpVector() + this->GetActorForwardVector() / 4 );
}

void AChest::BallDropDelayTimer()
{
	CallTracker--;
	if (CallTracker <=0)
	{
		//Gdy timer dobiegnie końca, piłka zostaje wyrzucona ze skrzyni
		DropBall();
		//Po wyrzuceniu piłki startuje kolejny timer, który opóźnia zamknięcie pokrywy - zapobiegnie to kolizji pokrywy z piłką oraz będzie wyglądać fajniej
		GetWorldTimerManager().ClearTimer(TimerHandle);
		CallTracker = CloseLidDelayTimerRemainingCalls;
		GetWorldTimerManager().SetTimer(TimerHandle, this, &AChest::CloseLidDelayTimer, 0.3f, true, 0.f);
	}
}

void AChest::CloseLidDelayTimer()
{
	CallTracker--;
	if (CallTracker <=0)
	{
		//Gdy timer dobiegnie końca, pokrywa zacznie się zamykać, a skrzynia ponownie może kolidować z piłkami
		ContainerMesh->SetCollisionProfileName("BlockAllDynamic");
		LidMesh->SetCollisionProfileName("BlockAllDynamic");
		bTestBoolRotation = true;
		GetWorldTimerManager().ClearTimer(TimerHandle);
	}
}

void AChest::Interaction()
{
	//Funkcja interakcji z interfejsu
	if (!bInteractionHintBlocked)
	{
		IInteractionInterface::Interaction();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("Interaction"));
		bTestBoolRotation = true;
		SetInteractHintVisibility(false);
		bInteractionHintBlocked = true;
	}
}

void AChest::SetInteractHintVisibility(bool bIsVisible)
{
	//Ustawienie widoczności indykatora interakcji
	if (!bInteractionHintBlocked)
	{
		IInteractionInterface::SetInteractHintVisibility(bIsVisible);
		if(InteractionWidgetComponent) Cast<UInteractionWidget>(InteractionWidgetComponent->GetWidget())->SetHintVisibility(bIsVisible);
	}
}



