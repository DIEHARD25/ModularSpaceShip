// Fill out your copyright notice in the Description page of Project Settings.


#include "MSP_PlayerController.h"


AMSP_PlayerController::AMSP_PlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;

	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

// Called when the game starts or when spawned
void AMSP_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameAndUI DefaultInput;
	DefaultInput.SetWidgetToFocus (nullptr);
	DefaultInput.SetHideCursorDuringCapture(false);
	DefaultInput.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);

	this->SetInputMode(DefaultInput);

	SelectionHUDPtr = Cast<AMSP_HUD_Canvas>(GetHUD());
}

// Called every frame
void AMSP_PlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMSP_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("LMB", IE_Pressed, this, &AMSP_PlayerController::SelectionPressed);
	InputComponent->BindAction("LMB", IE_Released, this, &AMSP_PlayerController::SelectionReleased);
	InputComponent->BindAction("RMB", IE_Pressed, this, &AMSP_PlayerController::MoveReleased);
}

void AMSP_PlayerController::SelectionPressed()
{
	UE_LOG(LogTemp, Warning, TEXT("SelectionPressed"));

	SelectionHUDPtr->SelectionPointStart = SelectionHUDPtr->GetMousePosition2D();
	SelectionHUDPtr->bIsSelectionMode = true;
}

void AMSP_PlayerController::SelectionReleased()
{
	UE_LOG(LogTemp, Warning, TEXT("SelectionReleased"));
	SelectionHUDPtr->SelectionPointEnd = SelectionHUDPtr->GetMousePosition2D();
	SelectionHUDPtr->bIsSelectionMode = false;

	SelectedShips = SelectionHUDPtr->FoundShips;
	UE_LOG(LogTemp, Warning, TEXT("SelectionReleased. FoundShipsNum: %d"), SelectedShips.Num());
}

void AMSP_PlayerController::MoveReleased()
{
	UE_LOG(LogTemp, Warning, TEXT("MoveReleased"));

	FVector DesiredLocation = MoveGoalLocation;

	if (MoveGoalLocation == FVector(0.0f, 0.0f, 0.0f))
	{
		FHitResult Hit;
		GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);

		DesiredLocation = Hit.Location; 
	}
	UE_LOG(LogTemp, Warning, TEXT("HitLocation: %f %f %f"), DesiredLocation.X, DesiredLocation.Y, DesiredLocation.Z);

	if (SelectedShips.Num())
	{

		// direction of movement
		FVector Direction = DesiredLocation - SelectedShips[0]->GetActorLocation();
		// rotator of movement order
		FRotator DirectionRotator = UKismetMathLibrary::MakeRotFromX(Direction);
		// now let's calculate left & right offsets using rotator
		float CosValue = FMath::Cos(FMath::DegreesToRadians(DirectionRotator.Yaw + 90.0f));
		float SinValue = FMath::Sin(FMath::DegreesToRadians(DirectionRotator.Yaw + 90.0f));

		FVector BaseOffsetVector(100.0f * CosValue, 100.0f * SinValue, 0.0f);

		
		// however, this is double-check since the arrays should be the same
		//SelectionHUDPtr->FoundShips = SelectedShips;
		SelectionHUDPtr->bIsMoveDrawMode = true;
		SelectionHUDPtr->FoundShipsTargetLocation.Empty();

		for (int32 i = 0; i < SelectedShips.Num(); ++i)
		{			
			AMSP_SpaceShipController * CurrentController = Cast<AMSP_SpaceShipController>(SelectedShips[i]->GetController());
			if (CurrentController)
			{
				FVector Location = DesiredLocation + (i % 2 == 0 ? (BaseOffsetVector)* i : (-BaseOffsetVector) * i);
				SelectionHUDPtr->FoundShipsTargetLocation.Add(Location);
				CurrentController->MoveToLocation(Location);
			}
		}
		//SelectionHUDPtr->bIsMoveDrawMode = false;
	}	
}

