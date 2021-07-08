// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"


UMainMenu::UMainMenu(const FObjectInitializer& MovieSceneBlends): UUserWidget(MovieSceneBlends)
{
	static const ConstructorHelpers::FClassFinder<UUserWidget> RowClassFinder(TEXT("/Game/MenuSystem/WBP_ServerRow"));
	ServerRowClass = RowClassFinder.Class;
}


void UMainMenu::Setup()
{
	UWorld* World = GetWorld();
	if (World == nullptr) { return; }

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (PlayerController == nullptr) { return; }

	AddToViewport();
	bIsFocusable = true;

	FInputModeUIOnly InputModeUIOnly;
	InputModeUIOnly.SetWidgetToFocus(TakeWidget());
	InputModeUIOnly.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PlayerController->SetInputMode(InputModeUIOnly);
	PlayerController->bShowMouseCursor = true;
}

void UMainMenu::Teardown()
{
	UWorld* World = GetWorld();
	if (World == nullptr) { return; }

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (PlayerController == nullptr) { return; }

	RemoveFromViewport();

	FInputModeGameOnly InputModeGameOnly;
	PlayerController->SetInputMode(InputModeGameOnly);
	PlayerController->bShowMouseCursor = false;
}

void UMainMenu::SetServerList(const TArray<FServerData>& Servers)
{
	ServerList->ClearChildren();
	SelectedIndex.Reset();

	uint32 Index = 0;
	for (const FServerData& Server : Servers)
	{
		UServerRowWidget* NewServer = CreateWidget<UServerRowWidget>(this, ServerRowClass);
		NewServer->Setup(this, Index);

		const FString ServerName = FString::Printf(TEXT("[%s] %s"), *Server.HostUserName, *Server.Name);
		NewServer->ServerName->SetText(FText::FromString(ServerName));

		const FString ConnectionFraction = FString::Printf(TEXT("%lu/%lu"), Server.CurrentPlayers, Server.MaxPlayers);
		NewServer->ConnectionFraction->SetText(FText::FromString(ConnectionFraction));

		ServerList->AddChild(NewServer);

		Index++;
	}
}

void UMainMenu::SelectIndex(uint32 Index)
{
	if (SelectedIndex.IsSet())
	{
		UWidget* Child = ServerList->GetChildAt(SelectedIndex.GetValue());
		if (auto ServerRow = Cast<UServerRowWidget>(Child))
		{
			ServerRow->bSelected = false;
		}
	}

	SelectedIndex = Index;

	if (auto ServerRow = Cast<UServerRowWidget>(ServerList->GetChildAt(SelectedIndex.GetValue())))
	{
		ServerRow->bSelected = true;
	}
}

bool UMainMenu::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (bSuccess)
	{
		if (HostButton) HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);
		if (JoinButton) JoinButton->OnClicked.AddDynamic(this, &UMainMenu::GoToJoinMenu);
		if (CancelJoinButton) CancelJoinButton->OnClicked.AddDynamic(this, &UMainMenu::CancelJoin);
		if (JoinServerButton) JoinServerButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);
	}
	return bSuccess;
}

void UMainMenu::HostServer()
{
	if (MenuInterface) { MenuInterface->HostServer(); }
}

void UMainMenu::GoToJoinMenu()
{
	if (MenuSwitcher)
	{
		MenuSwitcher->SetActiveWidget(JoinMenuWidget);
		MenuInterface->RefreshSessions();
	}
}

void UMainMenu::CancelJoin()
{
	if (MenuSwitcher) { MenuSwitcher->SetActiveWidget(MainMenuWidget); }
}

void UMainMenu::JoinServer()
{
	if (SelectedIndex.IsSet() && MenuInterface)
	{
		MenuInterface->JoinServer(SelectedIndex.GetValue());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected index is not set"))
	}
}
