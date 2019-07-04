// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSExtractionZone.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FPSCharacter.h"
#include "FPSGameMode.h"



// Sets default values
AFPSExtractionZone::AFPSExtractionZone()
{
	PrimaryActorTick.bCanEverTick = true;

    OverlapComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComponent"));
    OverlapComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    OverlapComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
    OverlapComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    OverlapComponent->SetBoxExtent(FVector(200.0f));
    RootComponent = OverlapComponent;

    OverlapComponent->OnComponentBeginOverlap.AddDynamic(this, &AFPSExtractionZone::HandleOverlap);

    DecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent"));
    DecalComponent->DecalSize = FVector(200.0f);
    DecalComponent->SetupAttachment(RootComponent);
}

void AFPSExtractionZone::HandleOverlap(
        UPrimitiveComponent *OverlappedComp,
        AActor *OtherActor,
        UPrimitiveComponent *OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult &SweepResult
) {

    auto* MyCharacter = Cast<AFPSCharacter>(OtherActor);
    if (MyCharacter == nullptr)
        return;

    UE_LOG(LogTemp, Log, TEXT("Overlapped with extraction zone"))

    if (MyCharacter->bIsCarryingObjective)
    {
        auto* GameMode = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
        if (GameMode)
        {
            GameMode->CompleteMission(MyCharacter, true);
        }

        return;
    }

    if (ObjectiveMissingSound)
    {
        UGameplayStatics::PlaySound2D(this, ObjectiveMissingSound);
    }
}