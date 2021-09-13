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

	if (SearchSubClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("NavVolume found!"));
		CurrentNavVolume = Cast<ANavigationVolume3D>(UGameplayStatics::GetActorOfClass(GetWorld(), SearchSubClass));
	}
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
	
	FHitResult Hit;
	GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
	AMSP_Camera * CameraPawn = Cast<AMSP_Camera>(GetPawn());
	if(CameraPawn)
		UE_LOG(LogTemp, Warning, TEXT("Cast OK"));

	//Hit.Location.Z = 0.0f;

	FVector DesiredLocation = Hit.Location + FVector(0.0f, 0.0f, CameraPawn->GetActorLocation().Z);

	UE_LOG(LogTemp, Warning, TEXT("HitLocation: %f %f %f"), DesiredLocation.X, DesiredLocation.Y, DesiredLocation.Z);

	if (SelectedShips.Num())
	{
		for (auto it : SelectedShips)
		{
			// movement call
			AMSP_SpaceShipController * CurrentController = Cast<AMSP_SpaceShipController>(it->GetController());
			if (CurrentController)
			{				
				UE_LOG(LogTemp, Warning, TEXT("Cast OK"));
				// find path
				TArray<FVector> CurrentPath = {};
				TArray<TEnumAsByte<EObjectTypeQuery> > SkippedObjectTypes;		
				SkippedObjectTypes.Add(TEnumAsByte<EObjectTypeQuery>(ECC_WorldStatic));
				SkippedObjectTypes.Add(TEnumAsByte<EObjectTypeQuery>(ECC_WorldDynamic));				
				CurrentNavVolume->FindPath(it->GetActorLocation(), DesiredLocation, SkippedObjectTypes, nullptr, CurrentPath);
				
				for (auto ot : CurrentPath)
					CurrentController->MoveToLocation(ot, 20.0f, false, false);

				/*
				if (EPathFollowingRequestResult::RequestSuccessful == CurrentController->MoveToLocation(DesiredLocation,-1.0f, false, true))
				{
					UE_LOG(LogTemp, Warning, TEXT("RequestSuccessful"));
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("RequestFailed"));
				}
				*/
			}
		}
				
	}
	
}