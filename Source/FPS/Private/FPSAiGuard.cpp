// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSAiGuard.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
AFPSAiGuard::AFPSAiGuard()
{
    // Set this character to call Tick() every frame. You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
    PawnSensingComponent->OnSeePawn.AddDynamic(this, &AFPSAiGuard::OnPawnSeen);
}

// Called when the game starts or when spawned
void AFPSAiGuard::BeginPlay()
{
    Super::BeginPlay();

}

void AFPSAiGuard::OnPawnSeen(APawn *SeenPawn)
{
    if (SeenPawn == nullptr)
    {
        return;
    }
    UE_LOG(LogTemp, Log, TEXT("Player spotted"))
    DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 32.f, 12, FColor::Yellow, false, 10.f);
}


// Called every frame
void AFPSAiGuard::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

