// Fill out your copyright notice in the Description page of Project Settings.


#include "Vehicule.h"

// Sets default values
AVehicule::AVehicule()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AVehicule::BeginPlay()
{
	Super::BeginPlay();
	vehiculeDataAsset->position = this->GetActorLocation();
	vehiculeDataAsset->orientation = FMatrix::Identity;
	
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
FVector AVehicule::Truncate(FVector steering_direction, float max_force) {
	float norm = steering_direction.Size();
	//si la norm du vecteur est plus petite ou égale à f
	if (norm <= max_force) {
		//on return le vecteur
		return steering_direction;
	}
	//sinon, retourner le vecteur normalisé * f (pour changer la taille du vecteur)
	return steering_direction.GetSafeNormal() * max_force;
}

void AVehicule::VehiculeMovement(FVector steering_direction) {
	FVector stearing_force = Truncate(steering_direction, vehiculeDataAsset->max_force);
	FVector acceleration = stearing_force / vehiculeDataAsset->mass;
	vehiculeDataAsset->velocity = Truncate(vehiculeDataAsset->velocity + acceleration, vehiculeDataAsset->max_speed);
	vehiculeDataAsset->position = vehiculeDataAsset->position + vehiculeDataAsset->velocity;
	this->SetActorLocation(vehiculeDataAsset->position);
	VehiculeOrientation();
}

//unreal utilise des angles, calcul fait avec des vecteurs faut transformer en angle
//(111) max, val entre 0 et 1
void AVehicule::VehiculeOrientation() {
	/*
	FVector forward = this->GetActorForwardVector();
	FVector ori = this->GetActorRotation().Vector();


	FVector new_forward = forward.GetSafeNormal();
	FVector approximate_up = FVector(0.0f, 0.0f, ori[2]);
	FVector new_side = FVector::CrossProduct(new_forward, approximate_up);
	FVector new_up = FVector::CrossProduct(new_forward, new_side);

	FMatrix m = FMatrix::Identity;
	m.SetAxes(&new_forward, &new_side, &new_up, &FVector::ZeroVector);
	FQuat newRotation = FQuat(m);
	this->SetActorRotation(newRotation);
	*/
	
	FVector new_forward = vehiculeDataAsset->velocity.GetSafeNormal();
	FVector approximate_up = FVector(0.0f, 0.0f, vehiculeDataAsset->position[2]).GetSafeNormal();
	FVector new_side = FVector::CrossProduct(new_forward, approximate_up);
	FVector new_up = FVector::CrossProduct(new_forward, new_side);

	vehiculeDataAsset->orientation.SetAxes(&new_forward, &new_side, &new_up, &FVector::ZeroVector);
	FQuat newRotation = FQuat(vehiculeDataAsset->orientation);
	this->SetActorRotation(newRotation);
	
}

FVector AVehicule::seek(AActor *target) {
	FVector targetPosition = target->GetActorLocation();
	FVector desired_velocity = (targetPosition - vehiculeDataAsset->position).GetSafeNormal() * vehiculeDataAsset->max_speed;
	return desired_velocity - vehiculeDataAsset->velocity;
}

//force->kg/m calculer en cm/s