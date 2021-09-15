// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AITypes.h"
#include "NavigationSystem.h"
#include "NavigationSystemTypes.h"
#include "MSP_SpaceShip.h"

#include "Kismet/KismetMathLibrary.h"

#include "MSP_SpaceShipController.generated.h"

/**
 * 
 */
UCLASS()
class MODULARSPACESHIP_API AMSP_SpaceShipController : public AAIController
{
	GENERATED_BODY()

public:
	AMSP_SpaceShipController();

	TArray<FVector> SearchPath(const FVector& location);

	virtual FPathFollowingRequestResult MoveTo(const FAIMoveRequest& MoveRequest, FNavPathSharedPtr* OutPath = nullptr) override;
};
