// Fill out your copyright notice in the Description page of Project Settings.


#include "MSP_Camera.h"

// Sets default values
AMSP_Camera::AMSP_Camera()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));

	CameraArm->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	PlayerCamera->AttachToComponent(CameraArm, FAttachmentTransformRules::KeepRelativeTransform);

	CameraArm->TargetArmLength = 1500.0f;
	// movement
	MovementSpeed = 1000.0f;
	MovementRestrictX = 25000.0f;
	MovementRestrictY = 25000.0f;
	// zoom
	ZoomSpeed = 4000.0f;
	ZoomLowRestrict = 300.0f;
	ZoomHighRestrict = 2500.0f;
	// turn
	TurnSpeed = 200.0f;
	TurnLeftRestrict = -120.0f;
	TurnRightRestrict = 120.0f;
}
void AMSP_Camera::HandleZoom(float InputAxis)
{
	if (InputAxis == 0.0f) return;
	
	CameraArm->TargetArmLength += InputAxis * ZoomSpeed * GetWorld()->GetDeltaSeconds();

	// restrict
	ApplyRestrict(CameraArm->TargetArmLength, ZoomLowRestrict, ZoomHighRestrict);
}
void AMSP_Camera::MoveUp(float InputAxis)
{
	if (InputAxis == 0.0f) return;

	float CurrentAngle = GetActorRotation().Yaw;
	// for X all ok with angle

	float Input = InputAxis * MovementSpeed * GetWorld()->GetDeltaSeconds();
	// ajust movement input with rotation
	SetActorLocation(AjustPositionByRotation(CurrentAngle, Input));
}


void AMSP_Camera::MoveRight(float InputAxis)
{
	if (InputAxis == 0.0f) return;
		
	float CurrentAngle = GetActorRotation().Yaw;
	// for Y we need to add 90 degrees
	CurrentAngle += 90.0f;

	float Input = InputAxis * MovementSpeed * GetWorld()->GetDeltaSeconds();
	// ajust movement input with rotation	
	SetActorLocation(AjustPositionByRotation(CurrentAngle, Input));
}
void AMSP_Camera::HandleTurn(float InputAxis)
{
	if (InputAxis == 0.0f) return;

	FRotator CurrentRotation = GetActorRotation();
	CurrentRotation.Yaw += InputAxis * TurnSpeed * GetWorld()->GetDeltaSeconds();

	// restrict
	ApplyRestrict(CurrentRotation.Yaw, TurnLeftRestrict, TurnRightRestrict);

	SetActorRotation(CurrentRotation);
}

// Called when the game starts or when spawned
void AMSP_Camera::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void AMSP_Camera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMSP_Camera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveUp"), this, &AMSP_Camera::MoveUp);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMSP_Camera::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("Zoom"), this, &AMSP_Camera::HandleZoom);
	PlayerInputComponent->BindAxis(TEXT("TurnCamera"), this, &AMSP_Camera::HandleTurn);
}

FVector AMSP_Camera::AjustPositionByRotation(float Angle, float Input)
{
	FVector CurrentLocation = GetActorLocation();
	CurrentLocation.Y += Input * FMath::Sin(FMath::DegreesToRadians(Angle));
	CurrentLocation.X += Input * FMath::Cos(FMath::DegreesToRadians(Angle));
	// restrict
	ApplyRestrict(CurrentLocation.X, -MovementRestrictX, MovementRestrictX);
	ApplyRestrict(CurrentLocation.Y, -MovementRestrictY, MovementRestrictY);

	return CurrentLocation;
}

void AMSP_Camera::ApplyRestrict(float & CheckedParam, float MinValue, float MaxValue)
{
	if (CheckedParam <= MinValue)
		CheckedParam = MinValue;
	else if(CheckedParam >= MaxValue)
		CheckedParam = MaxValue;
}
