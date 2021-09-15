// Fill out your copyright notice in the Description page of Project Settings.


#include "MSP_SpaceShipController.h"

AMSP_SpaceShipController::AMSP_SpaceShipController()
{
}

TArray<FVector> AMSP_SpaceShipController::SearchPath(const FVector& location)
{
	FPathFindingQuery Query;

	const FAIMoveRequest Request(location);

	const bool bValidQuery = BuildPathfindingQuery(Request, Query);

	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());

	FPathFindingResult PathResult;

	TArray<FVector> Result;

	if (NavSys)
	{
		PathResult = NavSys->FindPathSync(Query);

		if (PathResult.Result != ENavigationQueryResult::Error)
		{
			if (PathResult.IsSuccessful() && PathResult.Path.IsValid())
			{
				for (FNavPathPoint point : PathResult.Path->GetPathPoints())
				{
					Result.Add(point.Location);
				}
			}
		}
		else
			UE_LOG(LogTemp, Warning, TEXT("Pathfinding failed."));		
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("Can't find navigation system."));

	return Result;
}

FPathFollowingRequestResult AMSP_SpaceShipController::MoveTo(const FAIMoveRequest & MoveRequest, FNavPathSharedPtr * OutPath)
{
	UE_LOG(LogTemp, Warning, TEXT("MoveTo call: %f %f %f"), MoveRequest.GetGoalLocation().X, MoveRequest.GetGoalLocation().Y, MoveRequest.GetGoalLocation().Z);
	if (GetPawn())
	{
		AMSP_SpaceShip * Ship = Cast<AMSP_SpaceShip>(GetPawn());
		if (Ship)
		{
			FRotator CurrentShipRotator = Ship->GetActorRotation();
			FRotator DesiredRotation = UKismetMathLibrary::MakeRotFromX(MoveRequest.GetGoalLocation());
			if (FMath::IsNearlyEqual(CurrentShipRotator.Yaw, DesiredRotation.Yaw, 2.0f))
			{
				return Super::MoveTo(MoveRequest, OutPath);
			}
			else
			{
				float DesiredYaw = UKismetMathLibrary::RInterpTo_Constant(
					CurrentShipRotator,
					FRotator(0.0, DesiredRotation.Yaw, 0.0),
					GetWorld()->GetDeltaSeconds(),
					250.0f).Yaw;
				// apply Yaw
				Ship->SetActorRotation(FRotator(0.0, DesiredYaw, 0.0));
			}
		}
		//UE_LOG(LogTemp, Warning, TEXT("GetPawnRotator: %f %f %f"), GetPawn()->GetActorRotation().Pitch, GetPawn()->GetActorRotation().Yaw, GetPawn()->GetActorRotation().Roll);		
	}
	return FPathFollowingRequestResult();
}
