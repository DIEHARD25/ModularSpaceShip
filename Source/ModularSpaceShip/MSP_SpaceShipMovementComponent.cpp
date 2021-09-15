// Fill out your copyright notice in the Description page of Project Settings.


#include "MSP_SpaceShipMovementComponent.h"

UMSP_SpaceShipMovementComponent::UMSP_SpaceShipMovementComponent()
{

}

void UMSP_SpaceShipMovementComponent::RequestPathMove(const FVector& MoveInput)
{
	// called when we using acc & deacc values
	//UE_LOG(LogTemp, Warning, TEXT("RequestPathMove call: %f %f %f"), MoveInput.X, MoveInput.Y, MoveInput.Z);
	// before we will call Super::RequestPathMove let's turn out component to movement direction

	Super::RequestPathMove(MoveInput);
}

void UMSP_SpaceShipMovementComponent::RequestDirectMove(const FVector & MoveVelocity, bool bForceMaxSpeed)
{
	// called when we set up velocity directly
	//UE_LOG(LogTemp, Warning, TEXT("RequestDirectMove call: %f %f %f"), MoveVelocity.X, MoveVelocity.Y, MoveVelocity.Z);

	Super::RequestDirectMove(MoveVelocity, bForceMaxSpeed);
}
