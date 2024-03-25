// Fill out your copyright notice in the Description page of Project Settings.


#include "Vehicule.h"
#include "MPIAGameMode.h"
#include "MPIAPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Graph.h"
#include "GraphNode.h"
#include "PlayerTargetToSpawn.h"

// Sets default values
AVehicule::AVehicule()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	reachedIsDestination = false;
	circuitIndexToReach = 0;
}

// Called when the game starts or when spawned
void AVehicule::BeginPlay()
{
	Super::BeginPlay();
	position = this->GetActorLocation();
	orientation = FMatrix::Identity;
	
}

// Called every frame
void AVehicule::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AVehicule::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

//si norm vecteur plus petite que max (soit f) alors on return le vecteur sinon on prend vecteur=>normalise *max et donc vecteur norm max
//pour ne pas que le vecteur dépasse l'accelaration maximal du vehicule
// max_force = f, steering_direction=v
//Vehicule vers point arriver => vecteur, ce vecteur on verif qu'il est plus grand ou non que l'acceleration autorisé et donc si plus grand le mettre a la taille de l'acceleration
//pour eviter qu'il aille d'un seul coup au point et donc respecte pas l'acceleration etc (effet etrange sinon), et si plus petit que l'acceleration le laisser petit pour pas qu'il depasse
//le point d'arriver.
FVector AVehicule::Truncate(FVector steering_direction, float maxForce) {
	float norm = steering_direction.Size();
	//si la norm du vecteur est plus petite ou égale à f
	if (norm <= maxForce) {
		//on return le vecteur
		return steering_direction;
	}
	//sinon, retourner le vecteur normalisé * f (pour changer la taille du vecteur)
	return steering_direction.GetSafeNormal() * maxForce;
}

void AVehicule::VehiculeMovement(FVector steering_direction) {
	FVector stearing_force = Truncate(steering_direction, max_force);
	FVector acceleration = stearing_force / mass;
	velocity = Truncate(velocity + acceleration, max_speed);
	position = position + velocity;
	this->SetActorLocation(position);
	VehiculeOrientation();
}

//unreal utilise des angles, calcul fait avec des vecteurs faut transformer en angle
//(111) max, val entre 0 et 1
void AVehicule::VehiculeOrientation() {
	
	FVector new_forward = velocity.GetSafeNormal();
	FVector approximate_up = FVector(0.0f, 0.0f, position[2]).GetSafeNormal();
	FVector new_side = FVector::CrossProduct(new_forward, approximate_up).GetSafeNormal();
	FVector new_up = FVector::CrossProduct(new_forward, new_side).GetSafeNormal();

	orientation.SetAxes(&new_forward, &new_side, &new_up, &FVector::ZeroVector);
	FQuat newRotation = FQuat(orientation);
	this->SetActorRotation(newRotation);
	
}

FVector AVehicule::seek(AActor *target) {
	if (target) {
		FVector targetPosition = target->GetActorLocation();
		FVector desired_velocity = (targetPosition - position).GetSafeNormal() * max_speed;
		return desired_velocity - velocity;
	}
	return FVector::Zero();
}

//force->kg/m calculer en cm/s

FVector AVehicule::Flee(AActor* target) {
	FVector targetPosition = target->GetActorLocation();
	FVector desired_velocity = (targetPosition - position).GetSafeNormal() * -max_speed;
	return desired_velocity - velocity;
}

FVector AVehicule::Pursuit(AVehicule* target, float turningParameter) {
	float distance = (GetActorLocation() - target->GetActorLocation()).Size();
	float t = distance * turningParameter;
	FVector targetLocation = target->GetActorLocation();
	FVector targetVelocity = target->velocity;
	FVector targetFutureVelocity = targetVelocity * t;
	FVector targetFutureLocation = targetLocation + targetFutureVelocity;
	FVector desired_velocity = (targetFutureLocation - position).GetSafeNormal() * max_speed;
	return desired_velocity - velocity;
}

FVector AVehicule::Arrival(AActor* target, float slowing_distance) {
	if (target) {
		FVector targetPosition = target->GetActorLocation();
		FVector target_offset = targetPosition - position;
		float distance = target_offset.Size();
		float ramped_speed = max_speed * (distance / slowing_distance);
		float clipped_speed = FMath::Min(ramped_speed, max_speed);
		FVector desired_velocity = (clipped_speed / distance) * target_offset;
		return desired_velocity - velocity;
	}
	else return FVector::Zero();
		
}

TArray<AActor*> AVehicule::RemakeCircuitArray(TArray<AActor*> targets) {
	TArray<AActor*> tmpActor = targets;
	if (needresizeforcircuit && targets.Num()>0) {
		needresizeforcircuit = false;
		int targetIndex = 0;
		for (int i = 0; i < targets.Num(); i++) {
			APlayerTargetToSpawn* target = Cast<APlayerTargetToSpawn>(targets[i]);
			if (target) {
				targetIndex = i;
				break;
			}
		}
		AGraph* graph = Cast<AGraph>(UGameplayStatics::GetActorOfClass(GetWorld(), AGraph::StaticClass()));
		APlayerTargetToSpawn* targetFinalToSpawn = Cast<APlayerTargetToSpawn>(targets[targets.Num()-1]);
		APlayerTargetToSpawn* targetBeginToSpawn = Cast<APlayerTargetToSpawn>(targets[targetIndex]);

		if (targetFinalToSpawn != targetBeginToSpawn) {
			TArray<AActor*> tmp = graph->AStar(targetFinalToSpawn->ClosestGraphNode, targetBeginToSpawn->ClosestGraphNode);
			for (auto& elem : tmp) {
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Orange, elem->GetName());
				tmpActor.Add(elem);
			}
			return tmpActor;
		}
		else {
			return tmpActor;
		}
	}
	else {
		return tmpActor;
	}
	


}

bool AVehicule::Circuit(TArray<AActor*> targets) {
	if (circuitIndexToReach < targets.Num()) {
		if (targets[circuitIndexToReach]) {
			VehiculeMovement(seek(targets[circuitIndexToReach]));
			if ((GetActorLocation() - targets[circuitIndexToReach]->GetActorLocation()).Size() <= 500) {
				circuitIndexToReach++;
			}
			//return false;
			if (circuitIndexToReach == targets.Num() - 1) return true;
			else return false;
		}
		return false;
	}
	else {
		//for jusque premiere target et recup index

		for (int i = 0; i < targets.Num(); i++) {
			APlayerTargetToSpawn* target = Cast<APlayerTargetToSpawn>(targets[i]);
			if (target) {
				circuitIndexToReach = i;
				return false;
			}
		}
		circuitIndexToReach = 0;
		return false;
	}
}

void AVehicule::OneWay(TArray<AActor*> targets) {
	/*if (targets.Num()>0) {
		VehiculeMovement(Arrival(targets[circuitIndexToReach], 5000));
	}*/
	SeveralPoints(targets);
}

void AVehicule::SeveralPoints(TArray<AActor*> targets) {
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange, FString::Printf(TEXT("%d"), targets.Num()));
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange, reachedIsDestination?TEXT("DESTINATION REACHED"):TEXT("DESTINATION NOT REACHED"));

	if (!reachedIsDestination && targets.Num()>0) {
		if (Circuit(targets)) {
			reachedIsDestination = true;
		}
	}
	else {
		if (targets.Num()>0 && targets[targets.Num() - 1]) {
			VehiculeMovement(Arrival(targets[targets.Num() - 1], 5000));
		}
	}
}



void AVehicule::TwoWay(TArray<AActor*> targets) {
	if (circuitIndexToReach < targets.Num()) {
		if (circuitIndexToReach > -1) {
			VehiculeMovement(seek(targets[circuitIndexToReach]));
			if ((GetActorLocation() - targets[circuitIndexToReach]->GetActorLocation()).Size() <= 500) {
				if (!twoWayReverseSens) circuitIndexToReach++;
				else circuitIndexToReach--;
			}
		}
		else {
			circuitIndexToReach++;
			twoWayReverseSens = false;
		}
	}
	else {
		circuitIndexToReach--;
		twoWayReverseSens = true;
	}
}


TArray<AActor*> AVehicule::GraphPointsArray() {
	//new array with new points
	AMPIAGameMode* gamemode = Cast<AMPIAGameMode>(GetWorld()->GetAuthGameMode());
	AMPIAPlayerController* controller = Cast<AMPIAPlayerController>(GetWorld()->GetFirstPlayerController());
	//final array
	TArray<AActor*> finalArray;

	if (controller) {
		

		//get graph to access A* method
		AGraph* graph = Cast<AGraph>(UGameplayStatics::GetActorOfClass(GetWorld(), AGraph::StaticClass()));


		//Knowing the actual closest node from the vehicule
		TArray<AActor*> tmp;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGraphNode::StaticClass(), tmp);

		for (auto& n : tmp) {
			AGraphNode* node = Cast<AGraphNode>(n);
			if (!ClosestGraphNode) {
				ClosestGraphNode = node;
			}
			else {
				float oldDistance = (GetActorLocation() - ClosestGraphNode->GetActorLocation()).Size();
				float newDistance = (GetActorLocation() - node->GetActorLocation()).Size();
				if (oldDistance > newDistance) {
					ClosestGraphNode = node;
				}
			}
		}

		//save previous node (first is vehicule ClosestGraphNode)
		AGraphNode* previousClosestNode = ClosestGraphNode;

		//new array
		TArray<AActor*> tmpArray;

		//for each targets spawned by the player
		for (int i = 0; i < controller->targetsSpawned.Num(); i++) {
			APlayerTargetToSpawn* target = Cast<APlayerTargetToSpawn>(controller->targetsSpawned[i]);

			if (!target) {
				UE_LOG(LogTemp, Warning, TEXT("Target is null vehicule GraphPointsArray"));
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange, TEXT("Target is null vehicule GraphPointsArray"));
				return finalArray;
			}

			//A* vehicule ClosestGraphNode, target ClosestGraphNode
			tmpArray = graph->AStar(previousClosestNode, target->ClosestGraphNode);
			
			//add graph nodes
			for (auto& n : tmpArray) {
				finalArray.Add(n);
			}
			//Add the target
			finalArray.Add(controller->targetsSpawned[i]);
			//save target->ClosestGraphNode
			previousClosestNode = target->ClosestGraphNode;
		}
		newArrayTargets = finalArray;
		return finalArray;
	}
	newArrayTargets = finalArray;
	return finalArray;
}