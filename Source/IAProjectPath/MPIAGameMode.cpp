// Fill out your copyright notice in the Description page of Project Settings.


#include "MPIAGameMode.h"



AMPIAGameMode::AMPIAGameMode() {
	OneWayMod = true;
	SeveralPointMod = false;
	CircuitMod = false;
	Started = false;
}



void AMPIAGameMode::SetOneWayMod() {
	OneWayMod = true;
	SeveralPointMod = false;
	CircuitMod = false;
}


void AMPIAGameMode::SetSeveralMod() {
	OneWayMod = false;
	SeveralPointMod = true;
	CircuitMod = false;
}


void AMPIAGameMode::SetCircuitMod() {
	OneWayMod = false;
	SeveralPointMod = false;
	CircuitMod = true;
}