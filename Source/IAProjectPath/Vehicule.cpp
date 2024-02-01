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
	if (FMath::Min(norm, max_force) <= max_force) {
		//on return le vecteur
		return steering_direction;
	}
	//sinon, retourner le vecteur normalisé * f (pour changer la taille du vecteur)
	return steering_direction.GetSafeNormal() * max_force;
}

void AVehicule::VehiculeMovement(FVector steering_direction) {
	FVector stearing_force = Truncate(steering_direction, vehiculeDataAsset->max_force);
	FVector acceleration = stearing_force / vehiculeDataAsset->mass;
	vehiculeDataAsset->velocity = Truncate(vehiculeDataAsset->velocity, vehiculeDataAsset->max_speed);
	vehiculeDataAsset->position = vehiculeDataAsset->position + vehiculeDataAsset->velocity;
	this->SetActorLocation(vehiculeDataAsset->position);
}

void AVehicule::VehiculeOrientation() {
	FVector new_forward = vehiculeDataAsset->velocity.GetSafeNormal();
}

FVector AVehicule::seek(AActor *target) {
	FVector targetPosition = target->GetActorLocation();
	FVector desired_velocity = (targetPosition - vehiculeDataAsset->position).GetSafeNormal() * vehiculeDataAsset->max_speed;
	return desired_velocity - vehiculeDataAsset->velocity;
}