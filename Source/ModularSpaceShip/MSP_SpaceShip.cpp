// Fill out your copyright notice in the Description page of Project Settings.


#include "MSP_SpaceShip.h"

// Sets default values
AMSP_SpaceShip::AMSP_SpaceShip()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	ShipMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	MovementComp = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));
	MovementComp->SetUpdatedComponent(RootComponent);
	
	MovementComp->Activate();
}

// Called when the game starts or when spawned
void AMSP_SpaceShip::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMSP_SpaceShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMSP_SpaceShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

