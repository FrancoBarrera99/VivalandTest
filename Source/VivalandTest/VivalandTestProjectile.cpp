// Fill out your copyright notice in the Description page of Project Settings.


#include "VivalandTestProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "VivalandTestPawn.h"
#include "VivalandTestPlayerController.h"
#include "VivalandTestAIController.h"
#include "VivalandTestGameMode.h"

// Sets default values
AVivalandTestProjectile::AVivalandTestProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;

	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));
	}

	// Setup Collision Component
	if (CollisionComponent == nullptr)
	{
		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollisionComponent"));
		CollisionComponent->InitSphereRadius(15.0f);
		RootComponent = CollisionComponent;
	}

	// Setup Projectile Component
	if (ProjectileComponent == nullptr)
	{
		ProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		ProjectileComponent->InitialSpeed = 1000.0f;
		ProjectileComponent->MaxSpeed = 1000.0f;
		ProjectileComponent->ProjectileGravityScale = 0.0f;
	}

	// Setup Static Mesh Component
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

// Called when the game starts or when spawned
void AVivalandTestProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void AVivalandTestProjectile::NotifyActorBeginOverlap(AActor* OtherActor)
{
	AVivalandTestPawn* Character = Cast<AVivalandTestPawn>(OtherActor);
	AVivalandTestPawn* LocalOwner = Cast<AVivalandTestPawn>(GetOwner());

	if (HasAuthority() && Character != nullptr && LocalOwner != nullptr)
	{
		AVivalandTestPlayerController* CharacterPC = Cast<AVivalandTestPlayerController>(Character->GetController());
		AVivalandTestPlayerController* LocalOwnerPC = Cast<AVivalandTestPlayerController>(LocalOwner->GetController());

		if (CharacterPC != nullptr && LocalOwnerPC != nullptr)
		{
			if (CharacterPC->GetPlayerTeam() != LocalOwnerPC->GetPlayerTeam())
			{
				Server_NotifyPlayerHit(Character);
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

void AVivalandTestProjectile::Server_NotifyPlayerHit_Implementation(AVivalandTestPawn* Player)
{
	AVivalandTestGameMode* GameMode = Cast<AVivalandTestGameMode>(GetWorld()->GetAuthGameMode());
	AVivalandTestPlayerController* PlayerPC = Cast<AVivalandTestPlayerController>(Player->GetController());

	if (GameMode != nullptr && PlayerPC != nullptr)
	{
		EPlayerTeam PlayerTeam = PlayerPC->GetPlayerTeam();
		int32 ScoreToIncrease = 1;
		PlayerPC->IncreasePlayerScore(ScoreToIncrease);
		GameMode->RestartPlayer(PlayerPC->GetAIController());
	}
}
