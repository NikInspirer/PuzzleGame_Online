// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"

#include "PuzzleGameInstance.h"


// Sets default values
ALobbyGameMode::ALobbyGameMode()
{
	PrimaryActorTick.bCanEverTick = false;
	PlayerCount = 0;
	bUseSeamlessTravel = true;
}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	PlayerCount++;

	if (2 <= PlayerCount)
	{
		FTimerHandle Timer;
		GetWorldTimerManager().SetTimer(Timer, this, &ALobbyGameMode::StartGame, 5.0f);
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	PlayerCount--;
	
}

void ALobbyGameMode::StartGame()
{
	auto GI = Cast<UPuzzleGameInstance>(GetGameInstance());
	if (GI)
	{
		GI->StartSession();
		GetWorld()->ServerTravel("/Game/PuzzlePlatforms_Online/Game?listen");
	}
}


