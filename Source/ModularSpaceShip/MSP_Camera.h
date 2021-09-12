// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "MSP_Camera.generated.h"

UCLASS()
class MODULARSPACESHIP_API AMSP_Camera : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMSP_Camera();

	// components
	UPROPERTY(EditAnywhere)
		UCameraComponent * PlayerCamera;
	UPROPERTY(EditAnywhere)
		USpringArmComponent * CameraArm;
	
	APlayerController * CurrentPC;

	//  movement
	UPROPERTY(EditAnywhere)
		float MovementSpeed;
	UPROPERTY(EditAnywhere)
		float MovementRestrictX;
	UPROPERTY(EditAnywhere)
		float MovementRestrictY;

	void MoveUp(float InputAxis);
	void MoveRight(float InputAxis);
	// height

	UPROPERTY(EditAnywhere)
		float PlaneHeight;
	UPROPERTY(EditAnywhere)
		int32 NumOfPlanesHigh;
	UPROPERTY(EditAnywhere)
		int32 NumOfPlanesLow;

	int32 CurrentPlaneNum;;

	void ChangePlaneToHigher();
	void ChangePlaneToLower();
	void ResetToDefaultPlane();

	// zoom
	UPROPERTY(EditAnywhere)
		float ZoomSpeed;
	UPROPERTY(EditAnywhere)
		float ZoomLowRestrict;
	UPROPERTY(EditAnywhere)
		float ZoomHighRestrict;
	UPROPERTY(EditAnywhere)
		float DefaultZoom;

	void HandleZoom(float InputAxis);
	// turn
	UPROPERTY(EditAnywhere)
		float TurnSpeed;
	UPROPERTY(EditAnywhere)
		float TurnLeftRestrict;
	UPROPERTY(EditAnywhere)
		float TurnRightRestrict;
	UPROPERTY(EditAnywhere)
		FRotator DefaultTurn;

	float CosValueUp;
	float SinValueUp;
	float CosValueRight;
	float SinValueRight;

	void HandleTurn(float InputAxis);
	// utils
	void CalculateCosAndSinValues(FRotator CurrentRotator);
	void ApplyRestrict(float & CheckedParam, float MinValue, float MaxValue);
	void ResetCameraRotationAndZoom();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
