// Fill out your copyright notice in the Description page of Project Settings.


#include "MSP_PlayerController.h"


AMSP_PlayerController::AMSP_PlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;

	DefaultMouseCursor = EMouseCursor::Crosshairs;

	MoveGoalLocation = FVector(0.0f,0.0f,0.0f);
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

	InputComponent->BindAction("MoveToGoal", IE_Pressed, this, &AMSP_PlayerController::MoveGoalSelection);
	//InputComponent->BindAction("HeightAjust", IE_Pressed, this, &AMSP_PlayerController::HeightAjustMode);
}

void AMSP_PlayerController::SelectionPressed()
{
	UE_LOG(LogTemp, Warning, TEXT("SelectionPressed"));

	SelectionHUDPtr->SelectionPointStart = SelectionHUDPtr->GetMousePosition2D();
	SelectionHUDPtr->bIsSelectionMode = true;
	//if ((SelectionHUDPtr->bIsMoveSelectionMode || SelectionHUDPtr->bIsHeightSelectionMode) && SelectedShips.Num())
	if (SelectionHUDPtr->bIsMoveSelectionMode && SelectedShips.Num())
	{
		SelectionHUDPtr->bIsMoveSelectionMode = false;
		// spike. TODO: add OneParam Delegate here
		MoveGoalLocation = SelectionHUDPtr->MoveGoalEnd;
		MoveReleased();
		MoveGoalLocation = FVector(0.0f, 0.0f, 0.0f);
	}
}

void AMSP_PlayerController::SelectionReleased()
{
	UE_LOG(LogTemp, Warning, TEXT("SelectionReleased"));
	SelectionHUDPtr->SelectionPointEnd = SelectionHUDPtr->GetMousePosition2D();
	SelectionHUDPtr->bIsSelectionMode = false;
	SelectionHUDPtr->bIsMoveSelectionMode = false;
	//SelectionHUDPtr->bIsHeightSelectionMode = false;

	SelectedShips = SelectionHUDPtr->FoundShips;
	UE_LOG(LogTemp, Warning, TEXT("SelectionReleased. FoundShipsNum: %d"), SelectedShips.Num());
}

void AMSP_PlayerController::MoveReleased()
{
	UE_LOG(LogTemp, Warning, TEXT("MoveReleased"));

	SelectionHUDPtr->bIsMoveSelectionMode = false;
	//SelectionHUDPtr->bIsHeightSelectionMode = false;

	FVector DesiredLocation = MoveGoalLocation;

	if (MoveGoalLocation == FVector(0.0f, 0.0f, 0.0f))
	{
		FHitResult Hit;
		GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);

		DesiredLocation = Hit.Location; // +FVector(0.0f, 0.0f, SelectionHUDPtr->Height.Z);
		//SelectionHUDPtr->Height.Z = 0.0f;
	}
	UE_LOG(LogTemp, Warning, TEXT("HitLocation: %f %f %f"), DesiredLocation.X, DesiredLocation.Y, DesiredLocation.Z);

	if (SelectedShips.Num())
	{
		/*
		// here need to calc the offset for other ships
		FVector DirectionVector = DesiredLocation - SelectedShips[0]->GetActorLocation();
		FRotator DirectionRotator = UKismetMathLibrary::MakeRotFromX(DirectionVector);
		UE_LOG(LogTemp, Warning, TEXT("DirectionVector: %f %f %f"), DirectionVector.X, DirectionVector.Y, DirectionVector.Z);
		UE_LOG(LogTemp, Warning, TEXT("DirectionRotator: %f %f %f"), DirectionRotator.Pitch, DirectionRotator.Yaw, DirectionRotator.Roll);
		// let's calc sin ad cos for those rotator
		float DirectionRotatorSin = FMath::Sin(FMath::DegreesToRadians(DirectionRotator.Yaw));
		float DirectionRotatorCos = FMath::Cos(FMath::DegreesToRadians(DirectionRotator.Yaw));
		UE_LOG(LogTemp, Warning, TEXT("DirectionRotatorSin: %f"), DirectionRotatorSin);
		UE_LOG(LogTemp, Warning, TEXT("DirectionRotatorCos: %f"), DirectionRotatorCos);

		float DirectionRotatorSin90 = FMath::Sin(FMath::DegreesToRadians(DirectionRotator.Yaw + 90.0f));
		float DirectionRotatorCos90 = FMath::Cos(FMath::DegreesToRadians(DirectionRotator.Yaw + 90.0f));
		UE_LOG(LogTemp, Warning, TEXT("DirectionRotatorSin90: %f"), DirectionRotatorSin90);
		UE_LOG(LogTemp, Warning, TEXT("DirectionRotatorCos90: %f"), DirectionRotatorCos90);
		*/
		for (int32 i = 0; i < SelectedShips.Num(); ++i)
		{			
			AMSP_SpaceShipController * CurrentController = Cast<AMSP_SpaceShipController>(SelectedShips[i]->GetController());
			if (CurrentController)
			{
				//CurrentController->MoveToLocation(DesiredLocation + FVector(i * 100.0f * DirectionRotatorCos + i * 100.0f * DirectionRotatorCos90, i * 100.0f * DirectionRotatorSin + i * 100.0f * DirectionRotatorSin90, 0.0f));
				CurrentController->MoveToLocation(DesiredLocation);
				/*
				UE_LOG(LogTemp, Warning, TEXT("Cast OK"));
				// find path
				TArray<FVector> TmpArray = CurrentController->SearchPath(DesiredLocation);
				//for (auto ot : TmpArray)
				for(int32 i = 0; i < TmpArray.Num(); ++i)
				{
					UE_LOG(LogTemp, Warning, TEXT("Path point: %f %f %f"), ot.X, ot.Y, ot.Z);
					CurrentController->MoveToLocation(TmpArray[i], -1.0f, false, true);
				}		
				*/
			}
		}
				
	}
	
}

void AMSP_PlayerController::MoveGoalSelection()
{
	UE_LOG(LogTemp, Warning, TEXT("MoveGoalSelection"));
	SelectionHUDPtr->bIsMoveSelectionMode = true;
	if (SelectedShips.Num())
		SelectionHUDPtr->LeadingSpaceShip = SelectedShips[0];
	
}

/*
void AMSP_PlayerController::HeightAjustMode()
{
	UE_LOG(LogTemp, Warning, TEXT("HeightAjustMode"));
	SelectionHUDPtr->bIsHeightSelectionMode = true;
}
*/
