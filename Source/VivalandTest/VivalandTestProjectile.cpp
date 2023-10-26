// Fill out your copyright notice in the Description page of Project Settings.


#include "VivalandTestProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AVivalandTestProjectile::AVivalandTestProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

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
	}
}

// Called when the game starts or when spawned
void AVivalandTestProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVivalandTestProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

