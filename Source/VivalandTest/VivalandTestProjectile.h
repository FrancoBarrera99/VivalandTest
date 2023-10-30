// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VivalandTestProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class UStaticMeshComponent;
class AVivalandTestPawn;

UCLASS()
class VIVALANDTEST_API AVivalandTestProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AVivalandTestProjectile();
	void InitializeProjectile(TArray<AActor*> IgnoreActors);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	UFUNCTION(Server, Reliable)
	void Server_NotifyPlayerHit(AVivalandTestCharacter* AICharacter);
	void Server_NotifyPlayerHit_Implementation(AVivalandTestCharacter* AICharacter);

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, Category = Movement)
	UProjectileMovementComponent* ProjectileComponent;

	UPROPERTY(VisibleAnywhere, Category = Projectile)
	UStaticMeshComponent* StaticMeshComponent;
};
