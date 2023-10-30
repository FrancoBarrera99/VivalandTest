// Fill out your copyright notice in the Description page of Project Settings.

#include "VivalandTestPawn.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"
#include "VivalandTestAIController.h"
#include "VivalandTestCharacter.h"

AVivalandTestPawn::AVivalandTestPawn()
{
	PrimaryActorTick.bCanEverTick = false;
    bReplicates = true;

    USceneComponent* SceneRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
    SetRootComponent(SceneRootComponent);

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(GetRootComponent());
    CameraBoom->TargetArmLength = 900.0f;
    CameraBoom->SetRelativeRotation(FRotator(-65.0f, 0.0f, 0.0f));
    CameraBoom->bDoCollisionTest = false;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    CameraComponent->SetupAttachment(CameraBoom);
}

void AVivalandTestPawn::InitializePawn(AActor* NewFollowedActor)
{
    FollowedActor = NewFollowedActor;
}

void AVivalandTestPawn::BeginPlay()
{
    float TimerRate = 0.0166f;
    GetWorldTimerManager().SetTimer(FollowTimerHandle, this, &AVivalandTestPawn::HandleFollowActor, TimerRate, true);
}

void AVivalandTestPawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    DOREPLIFETIME(AVivalandTestPawn, FollowedActor);
}

void AVivalandTestPawn::HandleFollowActor()
{
    if (FollowedActor != nullptr)
    {
        float InterpolationSpeed = 10.0f;
        float DeltaTime = 0.1f;
        FVector NewLocation = FMath::VInterpTo(GetActorLocation(), FollowedActor->GetActorLocation(), DeltaTime, InterpolationSpeed);
        SetActorLocation(NewLocation);
    }
}