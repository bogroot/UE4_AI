// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
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

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void EnterBattleMode();
    virtual void EnterBattleMode_Implementation();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void QuitBattleMode();
    virtual void QuitBattleMode_Implementation();


public:
    int32 Timer;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BehaviorTree")
    UBehaviorTree* BehaviorTreeAsset;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BehaviorTree")
    UBlackboardComponent* Bboard;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Detect")
    int32 LineOfSightTimer;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Detect")
    float AttackDistance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detect")
    float Degree;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detect")
    float Distance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BehaviorTree")
    FName HasLineOfSight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BehaviorTree")
    FName EnemyActor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BehaviorTree")
    FName HasChasedPlayer;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BehaviorTree")
    FName BattleMode;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BehaviorTree")
    AActor* PlayerActor;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Detect")
    FName PlayerTag;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Patrol")
    bool IsRandomPatrol;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Patrol")
    float PatrolRadius;
};
