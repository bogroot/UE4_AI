// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "BehaviorTree/BehaviorTree.h"
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

protected:
    virtual void BeginPlay() override;
    
    void StartTimer();
 
public:
    virtual void Tick(float DeltaTime) override;

    virtual void OnPossess(APawn* InPawn) override;

    UFUNCTION(BlueprintCallable)
    void DetectPlayer();

    UFUNCTION(BlueprintCallable)
    void EnterBattleMode();

    UFUNCTION(BlueprintCallable)
    void QuitBattleMode();

public:
    int32 Timer;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UBehaviorTree* BehaviorTreeAsset;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UBlackboardComponent* Bboard;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float LineOfSightTimer;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    float AttackDistance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Degree;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Distance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool CanAttack;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName HasLineOfSight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName EnemyActor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName HasChasedPlayer;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName BattleMode;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    AActor* PlayerActor;

};
