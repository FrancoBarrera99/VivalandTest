// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "VivalandTestPlayerState.generated.h"

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
	FORCEINLINE EPlayerTeam GetPlayerTeam() const { return Team; };
	FORCEINLINE int32 GetPlayerScore() const { return Score; };
	void IncreasePlayerScore(int32 Value);

protected:
	EPlayerTeam Team;
	int32 Score;
	
};
