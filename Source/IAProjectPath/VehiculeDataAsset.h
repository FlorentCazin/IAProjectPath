// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Math/Vector.h"
#include "Chaos/Matrix.h"
#include "VehiculeDataAsset.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class IAPROJECTPATH_API UVehiculeDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="VehiculeProperty")
	float mass;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "VehiculeProperty")
	FVector position;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "VehiculeProperty")
	FVector velocity;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "VehiculeProperty")
	FMatrix orientation;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "VehiculeProperty")
	float max_force;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "VehiculeProperty")
	float max_speed;
};
