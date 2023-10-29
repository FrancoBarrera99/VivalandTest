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

EPlayerTeam AVivalandTestPlayerState::GetPlayerTeam() const
{
	return PlayerTeam;
}

int32 AVivalandTestPlayerState::GetPlayerScore() const
{
	return PlayerScore;
}

FText AVivalandTestPlayerState::GetPlayerUsername() const
{
	return PlayerUsername;
}

void AVivalandTestPlayerState::SetPlayerUsername(FText NewUsername)
{
	PlayerUsername = NewUsername;
}

void AVivalandTestPlayerState::SetPlayerTeam(EPlayerTeam NewTeam)
{
	PlayerTeam = NewTeam;
}

void AVivalandTestPlayerState::IncreasePlayerScore(int32 Value)
{
	PlayerScore += Value;
}

void AVivalandTestPlayerState::Server_InitializePlayer_Implementation(const FText& NewUsername, EPlayerTeam NewPlayerTeam)
{
	SetPlayerUsername(NewUsername);
	SetPlayerTeam(NewPlayerTeam);
}

void AVivalandTestPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME(AVivalandTestPlayerState, PlayerTeam);
	DOREPLIFETIME(AVivalandTestPlayerState, PlayerScore);
	DOREPLIFETIME(AVivalandTestPlayerState, PlayerUsername);
}
