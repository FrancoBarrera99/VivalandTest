// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "VivalandTestProjectile.generated.h"

// Forward Declarations
class AVivalandTestPawn;
class UProjectileMovementComponent;
class USphereComponent;
class UStaticMeshComponent;

/**
 *	The purpose of this class to handle collision with AVivalandTestCharacters as a projectile.
 */
UCLASS()
class VIVALANDTEST_API AVivalandTestProjectile : public AActor
{
	GENERATED_BODY()

public:
	AVivalandTestProjectile();
	void InitializeProjectile(TArray<AActor*> IgnoreActors);

protected:
	//Override Section
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	//Functions Section
	UFUNCTION(Server, Reliable)
	void Server_NotifyPlayerHit(AVivalandTestCharacter* AICharacter);
	void Server_NotifyPlayerHit_Implementation(AVivalandTestCharacter* AICharacter);

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* CollisionComponent;
	UPROPERTY(VisibleAnywhere, Category = Movement)
	UProjectileMovementComponent* ProjectileComponent;
	UPROPERTY(VisibleAnywhere, Category = Projectile)
	UStaticMeshComponent* StaticMeshComponent;
};
