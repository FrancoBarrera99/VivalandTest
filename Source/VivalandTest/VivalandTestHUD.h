// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "VivalandTestHUD.generated.h"

/**
 *	The purpose of this class is to instantiate GUIs and provide an interface to handle each one.
 */
UCLASS()
class VIVALANDTEST_API AVivalandTestHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
	void HideScoreboardGUI();
	UFUNCTION(BlueprintImplementableEvent)
	void HideTeamSelectGUI();
	UFUNCTION(BlueprintImplementableEvent)
	void ShowScoreboardGUI();
	UFUNCTION(BlueprintImplementableEvent)
	void ShowTeamSelectGUI();
};
