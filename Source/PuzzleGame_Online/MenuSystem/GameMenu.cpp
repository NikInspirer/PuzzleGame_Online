// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMenu.h"

bool UGameMenu::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (bSuccess)
	{
		if (BackButton) { BackButton->OnClicked.AddDynamic(this, &UGameMenu::BackToGame); }
		if (QuitButton) { QuitButton->OnClicked.AddDynamic(this, &UGameMenu::QuitGame); }
	}
	return bSuccess;
}

void UGameMenu::BackToGame()
{
	UE_LOG(LogTemp, Warning, TEXT("Back to game!"))
}

void UGameMenu::QuitGame()
{
	UE_LOG(LogTemp, Warning, TEXT("Quit game =("))
}

void UGameMenu::Setup()
{
	UWorld* World = GetWorld();
	if (World == nullptr) { return; }

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (PlayerController == nullptr) { return; }

	AddToViewport();

	FInputModeUIOnly InputModeUIOnly;
	InputModeUIOnly.SetWidgetToFocus(TakeWidget());
	InputModeUIOnly.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PlayerController->SetInputMode(InputModeUIOnly);
	PlayerController->bShowMouseCursor = true;
}