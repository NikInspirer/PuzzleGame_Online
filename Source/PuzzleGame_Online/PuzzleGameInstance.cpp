#include "PuzzleGameInstance.h"

#include "GameMapsSettings.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MenuSystem/MainMenu.h"

const static FName SESSION_NAME = TEXT("Game");
const static FName SERVER_NAME = TEXT("PuzzleGame_ServerName");

UPuzzleGameInstance::UPuzzleGameInstance(const FObjectInitializer& ObjectInitializer):
	UGameInstance(ObjectInitializer)
{
	UE_LOG(LogTemp, Warning, TEXT("UPuzzleGameInstance CONSTRUCTOR"))

	{
		const ConstructorHelpers::FClassFinder<UUserWidget> MenuClassBP(TEXT("/Game/MenuSystem/WBP_MainMenu"));
		if (MenuClassBP.Class)
		{
			MenuClass = MenuClassBP.Class;
			UE_LOG(LogTemp, Warning, TEXT("MenuClass = %s"), *MenuClass->GetName())
		}
	}

	{
		const ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuClassBP(TEXT("/Game/MenuSystem/WBP_GameMenu"));
		if (InGameMenuClassBP.Class)
		{
			InGameMenuClass = InGameMenuClassBP.Class;
			UE_LOG(LogTemp, Warning, TEXT("InGameMenuClass = %s"), *InGameMenuClass->GetName())
		}
	}
}

void UPuzzleGameInstance::Init()
{
	auto OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("FOUND %s OnlineSubsystem"), *OnlineSubsystem->GetSubsystemName().ToString())

		SessionInterfacePtr = OnlineSubsystem->GetSessionInterface();
		if (SessionInterfacePtr.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("FOUND Session"))
			SessionInterfacePtr->OnCreateSessionCompleteDelegates.AddUObject(
				this, &UPuzzleGameInstance::CreateSessionComplete);
			SessionInterfacePtr->OnDestroySessionCompleteDelegates.
			                     AddUObject(this, &UPuzzleGameInstance::DestroySessionComplete);
			SessionInterfacePtr->OnFindSessionsCompleteDelegates.AddUObject(
				this, &UPuzzleGameInstance::FindSessionsComplete);
			SessionInterfacePtr->OnJoinSessionCompleteDelegates.AddUObject(
				this, &UPuzzleGameInstance::JoinSessionComplete);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("DO NOT FOUND OnlineSubsystem"))
	}

	if (GEngine)
	{
		GEngine->OnNetworkFailure().AddUObject(this, &UPuzzleGameInstance::NetworkFailure);
	}
}

void UPuzzleGameInstance::LoadMainMenu()
{
	Menu = CreateWidget<UMainMenu>(this, MenuClass);
	if (Menu == nullptr) { return; }

	Menu->Setup();
	Menu->SetMenuInterface(this);
}

void UPuzzleGameInstance::LoadInGameMenu()
{
	InGameMenu = CreateWidget<UGameMenu>(this, InGameMenuClass);
	if (InGameMenu == nullptr) { return; }

	InGameMenu->Setup();
}

void UPuzzleGameInstance::HostServer()
{
	if (SessionInterfacePtr)
	{
		auto ExistingSession = SessionInterfacePtr->GetNamedSession(SESSION_NAME);
		if (ExistingSession)
		{
			SessionInterfacePtr->DestroySession(SESSION_NAME);
		}
		else
		{
			CreateSession();
		}
	}
}

void UPuzzleGameInstance::JoinServer(uint32 Index)
{
	if (!SessionInterfacePtr.IsValid() || !SessionSearchPtr.IsValid()) return;

	Menu->Teardown();
	SessionInterfacePtr->JoinSession(0, SESSION_NAME, SessionSearchPtr->SearchResults[Index]);
}

void UPuzzleGameInstance::RefreshSessions()
{
	UE_LOG(LogTemp, Warning, TEXT("START find session"))

	SessionSearchPtr = MakeShareable(new FOnlineSessionSearch());
	if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
	{
		SessionSearchPtr->bIsLanQuery = true;
	}
	else
	{
		SessionSearchPtr->bIsLanQuery = false;
	}
	SessionSearchPtr->MaxSearchResults = 100;
	SessionSearchPtr->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Type::Equals);

	SessionInterfacePtr->FindSessions(0, SessionSearchPtr.ToSharedRef());
}

void UPuzzleGameInstance::StartSession()
{
	if (SessionInterfacePtr.IsValid())
	{
		SessionInterfacePtr->StartSession(SESSION_NAME);
	}
}

void UPuzzleGameInstance::CreateSession()
{
	if (SessionInterfacePtr)
	{
		FOnlineSessionSettings Settings;
		if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
		{
			Settings.bIsLANMatch = true;
		}
		else
		{
			Settings.bIsLANMatch = false;
		}
		Settings.NumPublicConnections = 2;
		Settings.bShouldAdvertise = true;
		Settings.bUsesPresence = true;
		Settings.Set(SERVER_NAME, FString(TEXT("Nik's Puzzle Game")),
		             EOnlineDataAdvertisementType::Type::ViaOnlineServiceAndPing);

		SessionInterfacePtr->CreateSession(0, SESSION_NAME, Settings);
	}
}

void UPuzzleGameInstance::CreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		UE_LOG(LogTemp, Error, TEXT("Can non create Session"))
		return;
	}

	Menu->Teardown();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(0, 5, FColor::Green, TEXT("Hosting"));
		if (UWorld* World = GetWorld())
		{
			World->ServerTravel("/Game/PuzzlePlatforms_Online/Lobby?listen");
		}
	}
}

void UPuzzleGameInstance::DestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful) { CreateSession(); }
}

void UPuzzleGameInstance::FindSessionsComplete(bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("FINISH find session"))

	TArray<FServerData> Servers;
	for (const FOnlineSessionSearchResult& Result : SessionSearchPtr->SearchResults)
	{
		UE_LOG(LogTemp, Warning, TEXT("found session: %s"), *Result.GetSessionIdStr())

		FServerData Server;
		FString ServerName;
		if (Result.Session.SessionSettings.Get(SERVER_NAME, ServerName))
		{
			Server.Name = ServerName;
		}
		else
		{
			Server.Name = TEXT("UNKNOWN");
		}
		Server.MaxPlayers = Result.Session.SessionSettings.NumPublicConnections;
		Server.CurrentPlayers = Server.MaxPlayers - Result.Session.NumOpenPublicConnections;
		Server.HostUserName = Result.Session.OwningUserName;

		Servers.Emplace(Server);
	}
	if (Menu) { Menu->SetServerList(Servers); }
}

void UPuzzleGameInstance::JoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (Result == EOnJoinSessionCompleteResult::Type::Success)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(0, 5, FColor::Green,
			                                 FString::Printf(TEXT("Joining to %s"), *SessionName.ToString()));
		}

		FString Address;
		SessionInterfacePtr->GetResolvedConnectString(SessionName, Address);

		if (APlayerController* PlayerController = GetFirstLocalPlayerController())
		{
			PlayerController->ClientTravel(Address, TRAVEL_Absolute);
		}
	}
}

void UPuzzleGameInstance::NetworkFailure(UWorld* World, UNetDriver* NetworkDriver, ENetworkFailure::Type FailureType,
                                         const FString& ErrorString)
{
	if (auto PlayerController = GetFirstLocalPlayerController())
	{
		PlayerController->ClientTravel("/Game/MenuSystem/LoginMap", TRAVEL_Absolute);
	}
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(0, 5, FColor::White,
		                                 TEXT("You was disconected from server!"));
	}
}
