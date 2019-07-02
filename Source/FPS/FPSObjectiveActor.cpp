// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSObjectiveActor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FPSCharacter.h"


// Sets default values
AFPSObjectiveActor::AFPSObjectiveActor()
{
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    RootComponent = MeshComponent;

    SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
    SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    SphereComponent->SetupAttachment(MeshComponent);
}

// Called when the game starts or when spawned
void AFPSObjectiveActor::BeginPlay()
{
	Super::BeginPlay();

	PlayEffects();
}

void AFPSObjectiveActor::PlayEffects()
{
    UGameplayStatics::SpawnEmitterAtLocation(this, PickupFX, GetActorLocation());
}

void AFPSObjectiveActor::NotifyActorBeginOverlap(AActor* OtherActor) {
    Super::NotifyActorBeginOverlap(OtherActor);

    PlayEffects();

    auto* MyCharacter = Cast<AFPSCharacter>(OtherActor);
    if (MyCharacter)
    {
        MyCharacter->bIsCarryingObjective = true;
        Destroy();
    }
}