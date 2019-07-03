// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSLaunchPad.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"


// Sets default values
AFPSLaunchPad::AFPSLaunchPad()
{
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));

    RootComponent = MeshComponent;

    OverlapComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComponent"));
    OverlapComponent->OnComponentBeginOverlap.AddDynamic(this, &AFPSLaunchPad::HandleOverlap);
    OverlapComponent->SetupAttachment(RootComponent);

    LaunchPitchAngle = 45.0f;
    LaunchPower = 1200.0f;

}

void AFPSLaunchPad::HandleOverlap(
        UPrimitiveComponent *OverlappedComp,
        AActor *OtherActor,
        UPrimitiveComponent *OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult &SweepResult
)
{
    UE_LOG(LogTemp, Log, TEXT("LaunchPad pressed"))

    auto Rotation = this->GetActorRotation();
    Rotation.Pitch += LaunchPitchAngle;
    auto Impulse = Rotation.Vector() * LaunchPower;

    if (OtherActor == nullptr)
    {
        return;
    }

    UGameplayStatics::SpawnEmitterAtLocation(OtherActor, LaunchFX, OtherActor->GetActorLocation());

    auto *Character = Cast<ACharacter>(OtherActor);
    if (Character)
    {
        Character->LaunchCharacter(Impulse, true, true);

        return;
    }
    else if (OtherComp && OtherComp->IsSimulatingPhysics())
    {
        OtherComp->AddImpulse(Impulse, NAME_None, true);
    }
}

