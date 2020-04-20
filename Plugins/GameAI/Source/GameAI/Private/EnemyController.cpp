// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyController.h"
#include "Kismet/KismetMathLibrary.h"

AEnemyController::AEnemyController()
{
    PrimaryActorTick.bCanEverTick = true;
    Bboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Bboard"));
    Timer = 0;
    AttackDistance = 120.f;
    LineOfSightTimer = 0.f;
    Degree = 0.f;
    Distance = 0.f;
    CanAttack = false;
    HasLineOfSight = FName(TEXT("HasLineOfSight"));
    EnemyActor = FName(TEXT("EnemyActor"));
    HasChasedPlayer = FName(TEXT("HasChasedPlayer"));
    BattleMode = FName(TEXT("BattleMode"));
    PlayerActor = nullptr;
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
            CanAttack = false;
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
    if ((UKismetMathLibrary::Acos(FVector::DotProduct(enemyToPlayer, pawnForward)) <= Degree) && (size <= Distance))
    {
        PlayerActor = playerPawn;
        Timer = 0;
        Bboard->SetValueAsBool(HasLineOfSight, true);
        Bboard->SetValueAsObject(EnemyActor, PlayerActor);
    }
    else
    {
        if (++Timer >= 200)
        {
            Bboard->SetValueAsBool(HasLineOfSight, false);
            Bboard->SetValueAsObject(EnemyActor, nullptr);
            QuitBattleMode();
        }
    }
}

void AEnemyController::EnterBattleMode()
{
    if (Bboard)
    {
        Bboard->SetValueAsBool(BattleMode, true);
    }
}

void AEnemyController::QuitBattleMode()
{
    if (Bboard)
    {
        Bboard->SetValueAsBool(BattleMode, false);
    }
}

