// Fill out your copyright notice in the Description page of Project Settings.


#include "Vehicule.h"

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
//pour ne pas que le vecteur d�passe l'accelaration maximal du vehicule
// max_force = f, steering_direction=v
//Vehicule vers point arriver => vecteur, ce vecteur on verif qu'il est plus grand ou non que l'acceleration autoris� et donc si plus grand le mettre a la taille de l'acceleration
//pour eviter qu'il aille d'un seul coup au point et donc respecte pas l'acceleration etc (effet etrange sinon), et si plus petit que l'acceleration le laisser petit pour pas qu'il depasse
//le point d'arriver.
FVector AVehicule::Truncate(FVector steering_direction, float maxForce) {
	float norm = steering_direction.Size();
	//si la norm du vecteur est plus petite ou �gale � f
	if (norm <= maxForce) {
		//on return le vecteur
		return steering_direction;
	}
	//sinon, retourner le vecteur normalis� * f (pour changer la taille du vecteur)
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
	FVector targetPosition = target->GetActorLocation();
	FVector desired_velocity = (targetPosition - position).GetSafeNormal() * max_speed;
	return desired_velocity - velocity;
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
	FVector targetPosition = target->GetActorLocation();
	FVector target_offset = targetPosition - position;
	float distance = target_offset.Size();
	float ramped_speed = max_speed * (distance / slowing_distance);
	float clipped_speed = FMath::Min(ramped_speed, max_speed);
	FVector desired_velocity = (clipped_speed / distance) * target_offset;
	return desired_velocity - velocity;
}

bool AVehicule::Circuit(TArray<AActor*> targets) {
	if (circuitIndexToReach < targets.Num()) {
		VehiculeMovement(seek(targets[circuitIndexToReach]));
		if ((GetActorLocation() - targets[circuitIndexToReach]->GetActorLocation()).Size() <= 500) {
			circuitIndexToReach++;
		}
		if (circuitIndexToReach == targets.Num() - 1) return true;
		else return false;
	}
	else {
		circuitIndexToReach = 0;
		return false;
	}
}

void AVehicule::OneWay(TArray<AActor*> targets) {
	if (!reachedIsDestination && targets.Num()>0 && circuitIndexToReach<targets.Num()) {
		if (Circuit(targets)) {
			reachedIsDestination = true;
		}
	}
	else if(targets.Num() > 0 && circuitIndexToReach < targets.Num()) {
		VehiculeMovement(Arrival(targets[circuitIndexToReach], 5000));
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