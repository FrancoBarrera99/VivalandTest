// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"

#include "VivalandTestGameMode.generated.h"

// Forward Declarations
class AVivalandTestCharacter;
enum class EPlayerTeam : uint8;

/**
 *	The purpose of this class is to control game related rules
 */
UCLASS(minimalapi)
class AVivalandTestGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AVivalandTestGameMode();
	void RestartPlayer(AController* NewPlayer) override;
};



