// Fill out your copyright notice in the Description page of Project Settings.


#include "VivalandTestProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "VivalandTestCharacter.h"
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

// Called when the game starts or when spawned
void AVivalandTestProjectile::BeginPlay()
{
	Super::BeginPlay();
	if (GetOwner() != nullptr)
	{
		CollisionComponent->IgnoreActorWhenMoving(Owner, true);
	}
}

void AVivalandTestProjectile::NotifyActorBeginOverlap(AActor* OtherActor)
{
	AVivalandTestCharacter* Character = Cast<AVivalandTestCharacter>(OtherActor);
	AVivalandTestCharacter* LocalOwner = Cast<AVivalandTestCharacter>(GetOwner());

	if (HasAuthority() && Character != nullptr && LocalOwner != nullptr)
	{
		if (Character->GetPlayerTeam() == LocalOwner->GetPlayerTeam())
		{
			Server_NotifyPlayerHit(Character);
		}
	}
	Destroy();
}

void AVivalandTestProjectile::Server_NotifyPlayerHit_Implementation(AVivalandTestCharacter* Player)
{
	AVivalandTestGameMode* GameMode = Cast<AVivalandTestGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode != nullptr)
	{
		int32 ScoreToIncrease = 1;
		GameMode->IncreaseTeamScore(Player->GetPlayerTeam(), ScoreToIncrease);
	}

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Server Increase Score"));
}
