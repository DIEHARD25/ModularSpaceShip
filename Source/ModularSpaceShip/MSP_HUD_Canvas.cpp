// Fill out your copyright notice in the Description page of Project Settings.


#include "MSP_HUD_Canvas.h"

AMSP_HUD_Canvas::AMSP_HUD_Canvas()
{
	bIsSelectionMode = false;
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
}

FVector2D AMSP_HUD_Canvas::GetMousePosition2D()
{
	float PosX, PosY;
	GetOwningPlayerController()->GetMousePosition(PosX, PosY);

	return FVector2D(PosX, PosY);
}