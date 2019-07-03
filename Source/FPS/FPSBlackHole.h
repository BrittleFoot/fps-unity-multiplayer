// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSBlackHole.generated.h"


class UStaticMeshComponent;

class USphereComponent;

class UParticleSystem;


UCLASS()

class FPS_API AFPSBlackHole : public AActor {
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AFPSBlackHole();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    UStaticMeshComponent *VisualMesh;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    USphereComponent *AreaOfEffect;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    USphereComponent *AreaOfCollapse;

    UPROPERTY(EditDefaultsOnly, Category = "Effects")
    UParticleSystem *CollapseFX;

    UPROPERTY(EditDefaultsOnly, Category = "Effects")
    float CollapseStrength;

    UFUNCTION()

    void CollapseOnOverlapInnerSphere(
            UPrimitiveComponent *OverlappedComp,
            AActor *OtherActor,
            UPrimitiveComponent *OtherComp,
            int32 OtherBodyIndex,
            bool bFromSweep,
            const FHitResult &SweepResult
    );

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

};
