// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSAiGuard.h"
#include "FPSGameMode.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/TargetPoint.h"
#include "AIController.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AFPSAiGuard::AFPSAiGuard()
{
    // Set this character to call Tick() every frame. You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    OriginalRotation = GetActorRotation();

    PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
    GuardState = EAIState::Idle;

    bForwardLoopTrace = true;
    bPathDone = false;

}

// Called when the game starts or when spawned
void AFPSAiGuard::BeginPlay()
{
    Super::BeginPlay();
    PawnSensingComponent->OnSeePawn.AddDynamic(this, &AFPSAiGuard::OnPawnSeen);
    PawnSensingComponent->OnHearNoise.AddDynamic(this, &AFPSAiGuard::OnNoiseHeard);

}

void AFPSAiGuard::OnPawnSeen(APawn *SeenPawn)
{
    if (SeenPawn == nullptr)
    {
        return;
    }
    SetGuardState(EAIState::Alerted);
    DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 32.f, 12, FColor::Red, false, 1.f, 0, 1.f);

    auto *GameMode = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
    if (GameMode)
    {
        GameMode->CompleteMission(SeenPawn, false);
    }
}


void AFPSAiGuard::OnNoiseHeard(APawn *NosieInstigator, const FVector &Location, float Volume)
{
    if (GuardState == EAIState::Alerted)
    {
        return;
    }

    if (NosieInstigator == nullptr)
    {
        return;
    }

    SetGuardState(EAIState::Suspicious);
    DrawDebugSphere(GetWorld(), Location, 32.f, 12, FColor::Green, false, 3.f, 0, 1.f);

    auto Direction = Location - GetActorLocation();
    Direction.Normalize();

    auto NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
    NewLookAt.Pitch = 0.f;
    NewLookAt.Roll = 0.f;

    SetActorRotation(NewLookAt);

    GetWorldTimerManager().ClearTimer(TH_ResetOrientation);
    GetWorldTimerManager().SetTimer(TH_ResetOrientation, this, &AFPSAiGuard::ResetOrientation, 3.0f);
}


void AFPSAiGuard::ResetOrientation()
{
    if (GuardState == EAIState::Alerted)
    {
        return;
    }
    SetGuardState(EAIState::Idle);
    SetActorRotation(OriginalRotation);
}


void AFPSAiGuard::SetGuardState(EAIState NewState)
{
    if (NewState == GuardState)
    {
        return;
    }

    GuardState = NewState;

    OnRep_GuardState();
}

void AFPSAiGuard::OnRep_GuardState()
{
    OnStateChanged(GuardState);
}

void AFPSAiGuard::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AFPSAiGuard, GuardState);
}


// Called every frame
void AFPSAiGuard::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (PatrolPath.Num() == 0)
        return;

    auto *AiController = Cast<AAIController>(Controller);
    if (AiController == nullptr)
        return;

    if (GuardState != EAIState::Idle)
    {
        AiController->StopMovement();
        return;
    }

    auto MoveRequestResult = AiController->MoveToActor(GetCurrentWaypoint(), 5.f);

    switch (MoveRequestResult)
    {
        case EPathFollowingRequestResult::AlreadyAtGoal:
            SkipWaypoint();
            break;
        case EPathFollowingRequestResult::Failed:
            UE_LOG(LogTemp, Warning, TEXT("Move Request failed to an actor"))
            SkipWaypoint();
            break;
        case EPathFollowingRequestResult::RequestSuccessful:
        default:
            break;
    }
}


ATargetPoint *AFPSAiGuard::GetCurrentWaypoint()
{
    return PatrolPath[CurrentPatrolWaypointIndex];
}

void AFPSAiGuard::SkipWaypoint()
{
    auto NumWaypoints = PatrolPath.Num();
    if (NumWaypoints < 2)
        return;

    if (PatrollingBehaviour == EAIPatrollingBehaviour::Loop)
    {
        CurrentPatrolWaypointIndex = (CurrentPatrolWaypointIndex + 1) % NumWaypoints;
        return;
    }

    if (PatrollingBehaviour == EAIPatrollingBehaviour::Once)
    {
        if (bPathDone)
            return;

        CurrentPatrolWaypointIndex += 1;
        if (CurrentPatrolWaypointIndex >= NumWaypoints) {
            CurrentPatrolWaypointIndex -= 1;
            bPathDone = true;
        }
        return;
    }

    if (bForwardLoopTrace)
    {
        auto next = CurrentPatrolWaypointIndex + 1;
        if (next >= NumWaypoints)
        {
            CurrentPatrolWaypointIndex -= 1;
            bForwardLoopTrace = !bForwardLoopTrace;
        }
        else
        {
            CurrentPatrolWaypointIndex = next;
        }
    }
    else
    {
        auto next = CurrentPatrolWaypointIndex - 1;
        if (next < 0)
        {
            CurrentPatrolWaypointIndex += 1;
            bForwardLoopTrace = !bForwardLoopTrace;
        }
        else
        {
            CurrentPatrolWaypointIndex = next;
        }
    }
}