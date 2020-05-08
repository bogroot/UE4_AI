// Fill out your copyright notice in the Description page of Project Settings.


#include "FriendController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Public/DrawDebugHelpers.h"

AFriendController::AFriendController()
{
    PrimaryActorTick.bCanEverTick = true;
    Bboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Bboard"));
    Timer = 0;
    AttackDistance = 160.f;
    LineOfSightTimer = 200;
    Degree = 30.f;
    Distance = 500.f;
    HasLineOfSight = FName(TEXT("HasLineOfSight"));
    EnemyActor = FName(TEXT("EnemyActor"));
    HasChasedEnemy = FName(TEXT("HasChasedEnemy"));
    BattleMode = FName(TEXT("BattleMode"));
    Enemy = nullptr;
    EnemyTag = "Enemy";
}

void AFriendController::BeginPlay()
{
    Super::BeginPlay();
    Bboard->InitializeBlackboard(*(BehaviorTreeAsset->BlackboardAsset));
}

void AFriendController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    DetectEnemy();
    if (Enemy && Bboard)
    {
        if ((Enemy->GetActorLocation() - AController::GetPawn()->GetActorLocation()).Size() <= AttackDistance)
        {
            Bboard->SetValueAsBool(HasChasedEnemy, true);
        }
        else
        {
            Bboard->SetValueAsBool(HasChasedEnemy, false);
        }
    }
}

void AFriendController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    RunBehaviorTree(BehaviorTreeAsset);
}

void AFriendController::DetectEnemy()
{
    APawn* controlledPawn = AController::GetPawn();
    FVector pawnForward = controlledPawn->GetActorForwardVector();
    FHitResult hitResult;
    FCollisionQueryParams queryParams = FCollisionQueryParams("", false, controlledPawn);
    FVector start = controlledPawn->GetActorLocation();
    FVector end = start + pawnForward * Distance;
    GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECollisionChannel::ECC_WorldStatic, queryParams);
    DrawDebugLine(GetWorld(), start, end, FColor(255, 0, 0), false, 0.0f, 0.0f, 10.0f);
    if (hitResult.GetActor() && hitResult.GetActor()->ActorHasTag(EnemyTag))
    {
        Enemy = hitResult.GetActor();
        Timer = 0;
        Bboard->SetValueAsBool(HasLineOfSight, true);
        Bboard->SetValueAsObject(EnemyActor, Enemy);
    }
    else
    {
        if (++Timer >= LineOfSightTimer)
        {
            Bboard->SetValueAsBool(HasLineOfSight, false);
            Bboard->SetValueAsObject(EnemyActor, nullptr);
            QuitBattleMode();
        }
    }
}

void AFriendController::EnterBattleMode_Implementation()
{
    if (Bboard)
    {
        Bboard->SetValueAsBool(BattleMode, true);
    }
}

void AFriendController::QuitBattleMode_Implementation()
{
    if (Bboard)
    {
        Bboard->SetValueAsBool(BattleMode, false);
    }
}
