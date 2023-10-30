// Fill out your copyright notice in the Description page of Project Settings.


#include "VivalandTestProjectile.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "VivalandTestAIController.h"
#include "VivalandTestCharacter.h"
#include "VivalandTestGameMode.h"
#include "VivalandTestPawn.h"
#include "VivalandTestPlayerController.h"
#include "VivalandTestPlayerState.h"

AVivalandTestProjectile::AVivalandTestProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));
	}
	if (CollisionComponent == nullptr)
	{
		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollisionComponent"));
		CollisionComponent->InitSphereRadius(15.0f);
		RootComponent = CollisionComponent;
	}
	if (ProjectileComponent == nullptr)
	{
		ProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		ProjectileComponent->InitialSpeed = 1000.0f;
		ProjectileComponent->MaxSpeed = 1000.0f;
		ProjectileComponent->ProjectileGravityScale = 0.0f;
	}
	if (StaticMeshComponent == nullptr)
	{
		StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
		StaticMeshComponent->SetupAttachment(GetRootComponent());
		StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AVivalandTestProjectile::InitializeProjectile(TArray<AActor*> IgnoreActors)
{
	for (auto Actor : IgnoreActors) 
	{
		if (Actor != nullptr)
		{
			CollisionComponent->IgnoreActorWhenMoving(Actor, true);
		}
	}
}

void AVivalandTestProjectile::NotifyActorBeginOverlap(AActor* OtherActor)
{
	AVivalandTestCharacter* AICharacter = Cast<AVivalandTestCharacter>(OtherActor);
	AVivalandTestPlayerController* LocalOwnerPC = Cast<AVivalandTestPlayerController>(GetOwner());

	if (HasAuthority() && AICharacter != nullptr && LocalOwnerPC != nullptr)
	{
		AVivalandTestPlayerController* AICharacterUserPC = AICharacter->GetUserPlayerController();

		if (AICharacterUserPC != nullptr && LocalOwnerPC != nullptr)
		{
			if (AICharacterUserPC->GetPlayerTeam() != LocalOwnerPC->GetPlayerTeam() && AICharacterUserPC->GetPlayerTeam() != EPlayerTeam::None)
			{
				Server_NotifyPlayerHit(AICharacter);
			}
		}
	}

	if (OtherActor != nullptr)
	{
		FString ActorName = OtherActor->GetName();
		FString Message = FString::Printf(TEXT("Projectile Overlap: %s"), *ActorName);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, Message);
	}
	Destroy();
}

void AVivalandTestProjectile::Server_NotifyPlayerHit_Implementation(AVivalandTestCharacter* AICharacter)
{
	AVivalandTestGameMode* GameMode = Cast<AVivalandTestGameMode>(GetWorld()->GetAuthGameMode());
	AVivalandTestPlayerController* AICharacterUserPC = AICharacter->GetUserPlayerController();

	if (GameMode != nullptr && AICharacterUserPC != nullptr)
	{
		EPlayerTeam PlayerTeam = AICharacterUserPC->GetPlayerTeam();
		int32 ScoreToIncrease = 1;
		AICharacterUserPC->IncreasePlayerScore(ScoreToIncrease);
		GameMode->RestartPlayer(AICharacter->GetController());
	}
}
