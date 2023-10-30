// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"

#include "VivalandTestPlayerState.generated.h"

// Dynamic Delegates Section
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerScoreUpdated, int32, NewScore);

// Enums Section
UENUM(BlueprintType)
enum class EPlayerTeam : uint8
{
	None,
	Red,
	Blue
};

/**
 *	The purpose of this class is to store player data and notify actualizations
 */
UCLASS()
class VIVALANDTEST_API AVivalandTestPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AVivalandTestPlayerState();
	void IncreasePlayerScore(int32 Value);
	UFUNCTION(BlueprintCallable)
	int32 GetPlayerScore() const;
	EPlayerTeam GetPlayerTeam() const;
	UFUNCTION(BlueprintCallable)
	FText GetPlayerUsername() const;
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_InitializePlayer(const FText& NewUsername, EPlayerTeam NewPlayerTeam);
	void Server_InitializePlayer_Implementation(const FText& NewUsername, EPlayerTeam NewPlayerTeam);
	UFUNCTION(BlueprintCallable)
	void SetPlayerUsername(FText NewUsername);
	UFUNCTION(BlueprintCallable)
	void SetPlayerTeam(EPlayerTeam NewTeam);
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnPlayerScoreUpdated OnPlayerScoreUpdated;

protected:
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
	UFUNCTION()
	void OnRep_PlayerScoreUpdated(int32 OldScore);

protected:
	UPROPERTY(ReplicatedUsing = OnRep_PlayerScoreUpdated)
	int32 PlayerScore;
	UPROPERTY(Replicated)
	EPlayerTeam PlayerTeam;
	UPROPERTY(Replicated)
	FText PlayerUsername;
};
