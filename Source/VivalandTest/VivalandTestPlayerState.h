// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "VivalandTestPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerScoreUpdated, int32, NewScore);

UENUM(BlueprintType)
enum class EPlayerTeam : uint8
{
	None,
	Red,
	Blue
};

/**
 * 
 */
UCLASS()
class VIVALANDTEST_API AVivalandTestPlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	AVivalandTestPlayerState();
	EPlayerTeam GetPlayerTeam() const;
	UFUNCTION(BlueprintCallable)
	int32 GetPlayerScore() const;
	UFUNCTION(BlueprintCallable)
	FText GetPlayerUsername() const;
	UFUNCTION(BlueprintCallable)
	void SetPlayerUsername(FText NewUsername);
	UFUNCTION(BlueprintCallable)
	void SetPlayerTeam(EPlayerTeam NewTeam);
	void IncreasePlayerScore(int32 Value);
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_InitializePlayer(const FText& NewUsername, EPlayerTeam NewPlayerTeam);
	void Server_InitializePlayer_Implementation(const FText& NewUsername, EPlayerTeam NewPlayerTeam);

	UPROPERTY(BlueprintAssignable)
	FOnPlayerScoreUpdated OnPlayerScoreUpdated;

protected:
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
	UFUNCTION()
	void OnRep_PlayerScoreUpdated(int32 OldScore);

	UPROPERTY(Replicated)
	EPlayerTeam PlayerTeam;
	UPROPERTY(ReplicatedUsing = OnRep_PlayerScoreUpdated)
	int32 PlayerScore;
	UPROPERTY(Replicated)
	FText PlayerUsername;
};
