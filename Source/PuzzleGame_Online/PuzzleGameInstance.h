#pragma once

#include "CoreMinimal.h"

#include "Interfaces/OnlineSessionInterface.h"
#include "MenuSystem/GameMenu.h"
#include "MenuSystem/MenuInterface.h"
#include "PuzzleGameInstance.generated.h"

class UMainMenu;

UCLASS()
class UPuzzleGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

public:
	explicit UPuzzleGameInstance(const FObjectInitializer& ObjectInitializer);
	virtual void Init() override;

	UFUNCTION(Exec, BlueprintCallable)
	void LoadMainMenu();

	UFUNCTION(Exec, BlueprintCallable)
	void LoadInGameMenu();

	UFUNCTION(Exec)
	virtual void HostServer() override;

	UFUNCTION(Exec)
	virtual void JoinServer(uint32 Index) override;

	UFUNCTION(Exec)
	virtual void RefreshSessions() override;

	void StartSession();


	UPROPERTY(VisibleAnywhere)
	TSubclassOf<UUserWidget> MenuClass;
	UPROPERTY(VisibleAnywhere)
	TSubclassOf<UUserWidget> InGameMenuClass;

	UPROPERTY()
	UMainMenu* Menu;
	UPROPERTY()
	UGameMenu* InGameMenu;

private:
	void CreateSession();

	void CreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void DestroySessionComplete(FName SessionName, bool bWasSuccessful);
	void FindSessionsComplete(bool bWasSuccessful);
	void JoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void NetworkFailure(UWorld* World, UNetDriver* NetworkDriver, ENetworkFailure::Type FailureType,
	                    const FString& ErrorString);

	IOnlineSessionPtr SessionInterfacePtr;
	TSharedPtr<FOnlineSessionSearch> SessionSearchPtr;
};
