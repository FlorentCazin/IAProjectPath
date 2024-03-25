// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GraphNode.h"
#include "Vehicule.generated.h"

UCLASS(Blueprintable)
class IAPROJECTPATH_API AVehicule : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVehicule();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "VehiculeProperty")
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
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "VehiculeProperty")

	int circuitIndexToReach;
	bool reachedIsDestination;
	bool twoWayReverseSens;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<AActor*> newArrayTargets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AGraphNode* ClosestGraphNode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool needresizeforcircuit = true;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Math")
	FVector Truncate(FVector steering_direction, float maxForce);

	UFUNCTION(BlueprintCallable, Category="AlgoDeplacement")
	void VehiculeMovement(FVector steering_direction);

	UFUNCTION(BlueprintCallable, Category="AlgoDeplacement")
	void VehiculeOrientation();

	UFUNCTION(BlueprintCallable, Category = "AlgoDeplacement")
	FVector seek(AActor *target);

	UFUNCTION(BlueprintCallable, Category = "AlgoDeplacement")
	FVector Flee(AActor* target);

	UFUNCTION(BlueprintCallable, Category = "AlgoDeplacement")
	FVector Pursuit(AVehicule* target, float turningParameter);

	UFUNCTION(BlueprintCallable, Category = "AlgoDeplacement")
	FVector Arrival(AActor* target, float slowing_distance);

	UFUNCTION(BlueprintCallable, Category = "AlgoDeplacement")
	bool Circuit(TArray<AActor*> targets);

	UFUNCTION(BlueprintCallable, Category = "AlgoDeplacement")
	void OneWay(TArray<AActor*> targets);

	UFUNCTION(BlueprintCallable, Category = "AlgoDeplacement")
	void TwoWay(TArray<AActor*> targets);

	UFUNCTION(BlueprintCallable, Category = "AlgoDeplacement")
	void SeveralPoints(TArray<AActor*> targets);

	UFUNCTION(BlueprintCallable, Category = "graph new array")
	TArray<AActor*> GraphPointsArray();

	UFUNCTION(BlueprintCallable, Category = "new array circuit")
	TArray<AActor*> RemakeCircuitArray(TArray<AActor*> targets);



};
