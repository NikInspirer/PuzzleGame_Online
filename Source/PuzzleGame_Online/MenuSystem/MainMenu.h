// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "MenuInterface.h"
#include "ServerRowWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Components/WidgetSwitcher.h"

#include "MainMenu.generated.h"

USTRUCT()
struct FServerData
{
	GENERATED_BODY()

	FString Name;
	FString HostUserName;
	uint32 CurrentPlayers;
	uint32 MaxPlayers;
};

/**
 * 
 */
UCLASS()
class PUZZLEGAME_ONLINE_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	explicit UMainMenu(const FObjectInitializer& MovieSceneBlends);
	
	void Setup();
	void Teardown();
	FORCEINLINE void SetMenuInterface(IMenuInterface* InMenuInterface) { MenuInterface = InMenuInterface; }
	void SetServerList(const TArray<FServerData>& Servers);
	void SelectIndex(uint32 Index);

protected:
	virtual bool Initialize() override;

private:
	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void GoToJoinMenu();

	UFUNCTION()
	void CancelJoin();

	UFUNCTION()
	void JoinServer();

	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
	UWidget* MainMenuWidget;

	UPROPERTY(meta = (BindWidget))
	UWidget* JoinMenuWidget;

	UPROPERTY(meta = (BindWidget))
	UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
	UButton* JoinButton;

	UPROPERTY(meta = (BindWidget))
	UButton* CancelJoinButton;

	UPROPERTY(meta = (BindWidget))
	UButton* JoinServerButton;

	UPROPERTY(meta = (BindWidget))
	UPanelWidget* ServerList;
	
	TSubclassOf<UUserWidget> ServerRowClass;

private:
	IMenuInterface* MenuInterface;
	TOptional<uint32> SelectedIndex;
};
