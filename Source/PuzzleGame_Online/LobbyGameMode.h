// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "PuzzleGame_OnlineGameMode.h"

#include "LobbyGameMode.generated.h"

UCLASS()
class PUZZLEGAME_ONLINE_API ALobbyGameMode : public APuzzleGame_OnlineGameMode
{
	GENERATED_BODY()

public:
	ALobbyGameMode();
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

private:
	uint32 PlayerCount;

	void StartGame();
};

