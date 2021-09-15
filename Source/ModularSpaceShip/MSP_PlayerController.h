// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

#include "MSP_SpaceShipController.h"
#include "MSP_HUD_Canvas.h"
#include "MSP_SpaceShip.h"
#include "MSP_Camera.h"

#include "MSP_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MODULARSPACESHIP_API AMSP_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMSP_PlayerController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupInputComponent() override;

	AMSP_HUD_Canvas * SelectionHUDPtr;

	TArray<AMSP_SpaceShip *> SelectedShips;

	void SelectionPressed();
	void SelectionReleased();

	void MoveReleased();
};
