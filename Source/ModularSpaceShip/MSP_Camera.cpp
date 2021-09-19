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
	DefaultZoom = 1500.0f;
	CameraArm->TargetArmLength = DefaultZoom;
	CameraArm->bDoCollisionTest = false;
	// movement
	MovementSpeed = 1000.0f;
	MovementRestrictX = 25000.0f;
	MovementRestrictY = 25000.0f;
	// height
	/*
	PlaneHeight = 500.0f;
	NumOfPlanesHigh = NumOfPlanesLow = 1;
	CurrentPlaneNum = 0;
	*/
	// zoom
	ZoomSpeed = 4000.0f;
	ZoomLowRestrict = 300.0f;
	ZoomHighRestrict = 2500.0f;
	// turn
	DefaultTurn = FRotator(0.0f,0.0f,0.0f);
	TurnSpeed = 200.0f;
	TurnLeftRestrict = -120.0f;
	TurnRightRestrict = 120.0f;

	CalculateCosAndSinValues(DefaultTurn);	
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
	PlayerInputComponent->BindAction(TEXT("ResetCamera"), IE_Pressed, this, &AMSP_Camera::ResetCameraRotationAndZoom);
	//PlayerInputComponent->BindAction(TEXT("PlaneUp"), IE_Pressed, this, &AMSP_Camera::ChangePlaneToHigher);
	//PlayerInputComponent->BindAction(TEXT("PlaneDown"), IE_Pressed, this, &AMSP_Camera::ChangePlaneToLower);

	PlayerInputComponent->BindAxis(TEXT("MoveUp"), this, &AMSP_Camera::MoveUp);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMSP_Camera::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("Zoom"), this, &AMSP_Camera::HandleZoom);
	PlayerInputComponent->BindAxis(TEXT("TurnCamera"), this, &AMSP_Camera::HandleTurn);
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

	float Input = InputAxis * MovementSpeed * GetWorld()->GetDeltaSeconds();

	FVector CurrentLocation = GetActorLocation();
	CurrentLocation.Y += Input * SinValueUp;
	CurrentLocation.X += Input * CosValueUp;
	// restrict
	ApplyRestrict(CurrentLocation.X, -MovementRestrictX, MovementRestrictX);
	ApplyRestrict(CurrentLocation.Y, -MovementRestrictY, MovementRestrictY);

	SetActorLocation(CurrentLocation);
}

void AMSP_Camera::MoveRight(float InputAxis)
{
	if (InputAxis == 0.0f) return;

	float Input = InputAxis * MovementSpeed * GetWorld()->GetDeltaSeconds();

	FVector CurrentLocation = GetActorLocation();
	CurrentLocation.Y += Input * SinValueRight;
	CurrentLocation.X += Input * CosValueRight;
	// restrict
	ApplyRestrict(CurrentLocation.X, -MovementRestrictX, MovementRestrictX);
	ApplyRestrict(CurrentLocation.Y, -MovementRestrictY, MovementRestrictY);

	SetActorLocation(CurrentLocation);
}

void AMSP_Camera::HandleTurn(float InputAxis)
{
	if (InputAxis == 0.0f) return;

	FRotator CurrentRotation = GetActorRotation();
	CurrentRotation.Yaw += InputAxis * TurnSpeed * GetWorld()->GetDeltaSeconds();
	// restrict
	ApplyRestrict(CurrentRotation.Yaw, TurnLeftRestrict, TurnRightRestrict);
	// calculate the sin & cos values and stor them till next change of turn
	CalculateCosAndSinValues(CurrentRotation);
	// apply the turn
	SetActorRotation(CurrentRotation);
}
/*
void AMSP_Camera::ChangePlaneToHigher()
{	
	if (CurrentPlaneNum + 1 > NumOfPlanesHigh)
		return;
	else
		++CurrentPlaneNum;

	FVector CurrentLocation = GetActorLocation();
	CurrentLocation.Z = PlaneHeight * CurrentPlaneNum;

	SetActorLocation(CurrentLocation);
}

void AMSP_Camera::ChangePlaneToLower()
{
	if (CurrentPlaneNum - 1 < -(NumOfPlanesLow))
		return;
	else
		--CurrentPlaneNum;

	FVector CurrentLocation = GetActorLocation();
	CurrentLocation.Z = PlaneHeight * CurrentPlaneNum;

	SetActorLocation(CurrentLocation);
}
*/
void AMSP_Camera::CalculateCosAndSinValues(FRotator CurrentRotator)
{
	CosValueUp = FMath::Cos(FMath::DegreesToRadians(CurrentRotator.Yaw));
	SinValueUp = FMath::Sin(FMath::DegreesToRadians(CurrentRotator.Yaw));
	CosValueRight = FMath::Cos(FMath::DegreesToRadians(CurrentRotator.Yaw + 90.0f));
	SinValueRight = FMath::Sin(FMath::DegreesToRadians(CurrentRotator.Yaw + 90.0f));
}

void AMSP_Camera::ApplyRestrict(float & CheckedParam, float MinValue, float MaxValue)
{
	if (CheckedParam <= MinValue)
		CheckedParam = MinValue;
	else if(CheckedParam >= MaxValue)
		CheckedParam = MaxValue;
}

void AMSP_Camera::ResetCameraRotationAndZoom()
{
	SetActorRotation(DefaultTurn);

	CalculateCosAndSinValues(DefaultTurn);

	CameraArm->TargetArmLength = DefaultZoom;
	//ResetToDefaultPlane();
}
/*
void AMSP_Camera::ResetToDefaultPlane()
{
	CurrentPlaneNum = 0;
	FVector CurrentLocation = GetActorLocation();
	CurrentLocation.Z = PlaneHeight * CurrentPlaneNum;

	SetActorLocation(CurrentLocation);
}
*/