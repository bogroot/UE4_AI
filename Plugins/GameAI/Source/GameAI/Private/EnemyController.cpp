// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Public/DrawDebugHelpers.h"

AEnemyController::AEnemyController()
{
    PrimaryActorTick.bCanEverTick = true;
    Bboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Bboard"));
    Timer = 0;
    AttackDistance = 120.f;
    LineOfSightTimer = 200;
    Degree = 30.f;
    Distance = 500.f;
    HasLineOfSight = FName(TEXT("HasLineOfSight"));
    EnemyActor = FName(TEXT("EnemyActor"));
    HasChasedPlayer = FName(TEXT("HasChasedPlayer"));
    BattleMode = FName(TEXT("BattleMode"));
    IsDead = FName(TEXT("IsDead"));
    PlayerActor = nullptr;
    PlayerTag = "Player";
    IsRandomPatrol = true;
    PatrolRadius = 500.0f;
}

void AEnemyController::BeginPlay()
{
    Super::BeginPlay();
    Bboard->InitializeBlackboard(*(BehaviorTreeAsset->BlackboardAsset));
}

void AEnemyController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    DetectPlayer();
    if (PlayerActor && Bboard)
    {
        if ((PlayerActor->GetActorLocation() - AController::GetPawn()->GetActorLocation()).Size() <= AttackDistance)
        {
            Bboard->SetValueAsBool(HasChasedPlayer, true);
        }
        else
        {
            Bboard->SetValueAsBool(HasChasedPlayer, false);
        }
    }
}

void AEnemyController::StartTimer()
{
    Bboard->SetValueAsBool(HasLineOfSight, false);
    Bboard->SetValueAsObject(EnemyActor, nullptr);
    QuitBattleMode();
}



void AEnemyController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    RunBehaviorTree(BehaviorTreeAsset);
}

void AEnemyController::DetectPlayer()
{
    APawn* playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    APawn* controlledPawn = AController::GetPawn();
    FVector pawnForward = controlledPawn->GetActorForwardVector();
    FVector enemyToPlayer = playerPawn->GetActorLocation()- controlledPawn->GetActorLocation();
    float size = enemyToPlayer.Size();
    enemyToPlayer.Normalize();

    FHitResult hitResult;
    FCollisionQueryParams queryParams = FCollisionQueryParams("", false, controlledPawn);
    FVector start = controlledPawn->GetActorLocation();
    FVector end = start + pawnForward * Distance;
    GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECollisionChannel::ECC_WorldStatic, queryParams );
    if ((UKismetMathLibrary::Acos(FVector::DotProduct(enemyToPlayer, pawnForward)) <= Degree) && (size <= Distance))
    {
        PlayerActor = playerPawn;
        Timer = 0;
        Bboard->SetValueAsBool(HasLineOfSight, true);
        Bboard->SetValueAsObject(EnemyActor, PlayerActor);
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

void AEnemyController::EnterBattleMode_Implementation()
{
    if (Bboard)
    {
        Bboard->SetValueAsBool(BattleMode, true);
    }
}

void AEnemyController::QuitBattleMode_Implementation()
{
    if (Bboard)
    {
        Bboard->SetValueAsBool(BattleMode, false);
    }
}

void AEnemyController::Dead_Implementation()
{
    if (Bboard)
    {
        Bboard->SetValueAsBool(IsDead, true);
    }
}


