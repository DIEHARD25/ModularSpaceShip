// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "MSP_SpaceShipMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class MODULARSPACESHIP_API UMSP_SpaceShipMovementComponent : public UFloatingPawnMovement
{
	GENERATED_BODY()
	
public:
	UMSP_SpaceShipMovementComponent();
	// this call is applied when Controller use MoveToLocation() call	
	// apply movement logic here
	virtual void RequestPathMove(const FVector& MoveInput) override;

	virtual void RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed) override;
	// variables

};
