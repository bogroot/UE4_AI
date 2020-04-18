// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyController.generated.h"

/**
 * 
 */
UCLASS()
class GAMEAI_API AEnemyController : public AAIController
{
	GENERATED_BODY()
      
public:
    AEnemyController();

public:
    virtual void BeginPlay() override;

    virtual void Tick(float DeltaTime) override;

	
};
