// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MPIAPlayerCharacter.h"
#include "MPIAPlayerController.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class IAPROJECTPATH_API AMPIAPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMPIAPlayerController();

	//Array of targets to go
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FVector> LocationArray;

	//OneWay mode
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool OneWayMod;

	//OneWay mode
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool SeveralPointMod;

	//Circuit mode
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CircuitMod;

	AMPIAPlayerCharacter *Player;

	
public:
	UFUNCTION(BlueprintCallable)
	FHitResult OnClickGetSingleLineTraceByChannel();

	UFUNCTION(BlueprintCallable)
	void SetOneWayMod();

	UFUNCTION(BlueprintCallable)
	void SetSeveralMod();

	UFUNCTION(BlueprintCallable)
	void SetCircuitMod();

	UFUNCTION(BlueprintCallable)
	void SpawningItemToReach(FHitResult hit);
};


