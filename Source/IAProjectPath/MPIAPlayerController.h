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
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//TArray<FVector> LocationArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> targetsSpawned;

	AMPIAPlayerCharacter *Player;

    //onepoint = 0, severalspoints = 1, circuit = 2
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ModeChoosen;

protected:
	virtual void OnPossess(APawn* aPawn) override;
	
public:
	UFUNCTION(BlueprintCallable)
	FHitResult OnClickGetSingleLineTraceByChannel();

	

	UFUNCTION(BlueprintCallable)
	void SpawningItemToReach(FHitResult hit);

	UFUNCTION(BlueprintCallable)
	void ClickAction(FHitResult hit);
};


