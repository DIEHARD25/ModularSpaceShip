// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MSP_SpaceShip.h"
#include "MSP_HUD_Canvas.generated.h"

/**
 * 
 */
UCLASS()
class MODULARSPACESHIP_API AMSP_HUD_Canvas : public AHUD
{
	GENERATED_BODY()

public:
	AMSP_HUD_Canvas();

	virtual void DrawHUD() override; // tick for HUD
	
	FVector2D SelectionPointStart;
	FVector2D SelectionPointEnd;

	FVector2D GetMousePosition2D();

	bool bIsSelectionMode;

	TArray<AMSP_SpaceShip *> FoundShips;
};
