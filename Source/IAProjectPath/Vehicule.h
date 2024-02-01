// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "VehiculeDataAsset.h"
#include "Vehicule.generated.h"

UCLASS(Blueprintable)
class IAPROJECTPATH_API AVehicule : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVehicule();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vehicule Data")
	UVehiculeDataAsset* vehiculeDataAsset;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Math")
	FVector Truncate(FVector steering_direction, float max_force);

	UFUNCTION(BlueprintCallable, Category="AlgoDeplacement")
	void VehiculeMovement(FVector steering_direction);

	UFUNCTION(BlueprintCallable, Category="AlgoDeplacement")
	void VehiculeOrientation();

	UFUNCTION(BlueprintCallable, Category = "AlgoDeplacement")
	FVector seek(AActor *target);

};
