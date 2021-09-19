// Fill out your copyright notice in the Description page of Project Settings.


#include "MSP_HUD_Canvas.h"

AMSP_HUD_Canvas::AMSP_HUD_Canvas()
{
	bIsSelectionMode = false;
	bIsMoveSelectionMode = false;
	//bIsHeightSelectionMode = false;
	LeadingSpaceShip = nullptr;
}

void AMSP_HUD_Canvas::DrawHUD()
{
	if (bIsSelectionMode)
	{
		if(FoundShips.Num() != 0) FoundShips.Empty();
		SelectionPointEnd = GetMousePosition2D();
		DrawRect(FLinearColor(0, 0, 1, 0.15f), SelectionPointStart.X, SelectionPointStart.Y, (SelectionPointEnd.X - SelectionPointStart.X), (SelectionPointEnd.Y - SelectionPointStart.Y));
		GetActorsInSelectionRectangle<AMSP_SpaceShip>(SelectionPointStart, SelectionPointEnd, FoundShips, false, false);
	}

	if (bIsMoveSelectionMode)
	{
		if (LeadingSpaceShip)
		{
			MoveGoalEnd = GetMousePositionToWorld();
			Draw3DLine(LeadingSpaceShip->GetActorLocation(), MoveGoalEnd, FColor(0, 0, 1, 0.15f));
		}
	}
	/*
	if (bIsHeightSelectionMode)
	{
		if (LeadingSpaceShip)
		{
			bIsMoveSelectionMode = false;
			Draw3DLine(LeadingSpaceShip->GetActorLocation(), MoveGoalEnd, FColor(0, 0, 1, 0.15f));
			// let's calc vertical
			Height = GetMousePositionToWorld();
			Height.Z = Height.X * 0.5;
			Height.X = MoveGoalEnd.X;
			Height.Y = MoveGoalEnd.Y;
			Draw3DLine(MoveGoalEnd, Height, FColor(0, 0, 1, 0.15f));
		}
	}
	*/
}

FVector AMSP_HUD_Canvas::GetMousePositionToWorld()
{
	FHitResult Hit;
	GetOwningPlayerController()->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
	return Hit.Location;
}

FVector2D AMSP_HUD_Canvas::GetMousePosition2D()
{
	float PosX, PosY;
	GetOwningPlayerController()->GetMousePosition(PosX, PosY);

	return FVector2D(PosX, PosY);
}

