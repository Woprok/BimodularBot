// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/BBPawn.h"

// Sets default values
ABBPawn::ABBPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABBPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABBPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABBPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

