// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSAiGuard.h"
#include "FPSGameMode.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"


// Sets default values
AFPSAiGuard::AFPSAiGuard()
{
    // Set this character to call Tick() every frame. You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    OriginalRotation = GetActorRotation();

    PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
    GuardState = EAIState::Idle;

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

    auto* GameMode = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
    if (GameMode)
    {
        GameMode->CompleteMission(SeenPawn, false);
    }
}


void AFPSAiGuard::OnNoiseHeard(APawn* NosieInstigator, const FVector& Location, float Volume)
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

    OnStateChanged(NewState);
}


// Called every frame
void AFPSAiGuard::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

