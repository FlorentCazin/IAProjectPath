// Fill out your copyright notice in the Description page of Project Settings.


#include "MPIAPlayerController.h"

AMPIAPlayerController::AMPIAPlayerController() {
	PrimaryActorTick.bCanEverTick = true;
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
}




FHitResult AMPIAPlayerController::OnClickGetSingleLineTraceByChannel() {
	UWorld* World = GetWorld(); //world reference
	FHitResult Hit;
	if (World) {
		TArray<AActor*> ignoreActors;
		FVector startLocation = GetPawn()->GetActorLocation();
		FVector2D mousePosition;
		GetMousePosition(mousePosition.X, mousePosition.Y);
		FVector WorldPosition, WorldDirection;
		DeprojectScreenPositionToWorld(mousePosition.X, mousePosition.Y, WorldPosition, WorldDirection);
		FVector endLocation = WorldPosition + (WorldDirection * 10000.f);
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActors(ignoreActors);
		World->LineTraceSingleByChannel(
			Hit,
			startLocation,
			endLocation,
			ECC_Visibility,
			CollisionParams
		);
		return Hit;
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("GetWorld problem!"));
		return Hit;
	}
}