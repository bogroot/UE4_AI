// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyController.h"

AEnemyController::AEnemyController()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AEnemyController::BeginPlay()
{
    Super::BeginPlay();
}

void AEnemyController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

