// Fill out your copyright notice in the Description page of Project Settings.


#include "VivalandTestPlayerState.h"

#include "Net/UnrealNetwork.h"

AVivalandTestPlayerState::AVivalandTestPlayerState()
{
	bReplicates = true;
	PlayerUsername = FText::GetEmpty();
	PlayerScore = 0;
	PlayerTeam = EPlayerTeam::None;
}

void AVivalandTestPlayerState::IncreasePlayerScore(int32 Value)
{
	PlayerScore += Value;
	OnRep_PlayerScoreUpdated(PlayerScore);
}

int32 AVivalandTestPlayerState::GetPlayerScore() const
{
	return PlayerScore;
}

EPlayerTeam AVivalandTestPlayerState::GetPlayerTeam() const
{
	return PlayerTeam;
}

FText AVivalandTestPlayerState::GetPlayerUsername() const
{
	return PlayerUsername;
}

void AVivalandTestPlayerState::Server_InitializePlayer_Implementation(const FText& NewUsername, EPlayerTeam NewPlayerTeam)
{
	SetPlayerUsername(NewUsername);
	SetPlayerTeam(NewPlayerTeam);
}

void AVivalandTestPlayerState::SetPlayerUsername(FText NewUsername)
{
	PlayerUsername = NewUsername;
}


void AVivalandTestPlayerState::SetPlayerTeam(EPlayerTeam NewTeam)
{
	PlayerTeam = NewTeam;
}

void AVivalandTestPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME(AVivalandTestPlayerState, PlayerTeam);
	DOREPLIFETIME(AVivalandTestPlayerState, PlayerScore);
	DOREPLIFETIME(AVivalandTestPlayerState, PlayerUsername);
}

void AVivalandTestPlayerState::OnRep_PlayerScoreUpdated(int32 OldScore)
{
	OnPlayerScoreUpdated.Broadcast(PlayerScore);
}
