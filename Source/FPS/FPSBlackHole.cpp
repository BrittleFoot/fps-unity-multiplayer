// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSBlackHole.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine.h"

// Sets default values
AFPSBlackHole::AFPSBlackHole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualMesh"));
    VisualMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    RootComponent = VisualMesh;

    AreaOfEffect = CreateDefaultSubobject<USphereComponent>(TEXT("AreaOfEffect"));
    AreaOfEffect->SetSphereRadius(1000);
    AreaOfEffect->SetupAttachment(VisualMesh);

    AreaOfCollapse = CreateDefaultSubobject<USphereComponent>(TEXT("AreaOfCollapse"));
    AreaOfCollapse->SetupAttachment(VisualMesh);

    AreaOfCollapse->OnComponentBeginOverlap.AddDynamic(this, &AFPSBlackHole::CollapseOnOverlapInnerSphere);
}

// Called when the game starts or when spawned
void AFPSBlackHole::BeginPlay()
{
	Super::BeginPlay();
	
}


void AFPSBlackHole::CollapseOnOverlapInnerSphere(
        UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult)
{
    if (OtherActor && OtherComp && OtherComp->IsSimulatingPhysics())
    {
        OtherActor->Destroy();

        if (CollapseFX)
        {
            UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), CollapseFX, OtherActor->GetActorLocation());
        }
    }
}

// Called every frame
void AFPSBlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    TArray<UPrimitiveComponent*> OverlappingComps;
    AreaOfEffect->GetOverlappingComponents(OverlappingComps);

    for (auto* Component: OverlappingComps)
    {
        if (Component && Component->IsSimulatingPhysics())
        {
            Component->AddRadialForce(GetActorLocation(), AreaOfEffect->GetScaledSphereRadius(), CollapseStrength, ERadialImpulseFalloff::RIF_Linear, true);
        }
    }
}

