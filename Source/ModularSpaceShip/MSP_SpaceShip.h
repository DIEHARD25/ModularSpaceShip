// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

#include "MSP_SpaceShipMovementComponent.h"

#include "MSP_SpaceShip.generated.h"

UCLASS()
class MODULARSPACESHIP_API AMSP_SpaceShip : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMSP_SpaceShip();

	// components
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent * ShipMesh;
	
	UPROPERTY(EditAnywhere)
		UMSP_SpaceShipMovementComponent * MovementComp;
		

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual UPawnMovementComponent* GetMovementComponent() const override;
};
