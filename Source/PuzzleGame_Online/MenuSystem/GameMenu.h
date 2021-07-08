// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"

#include "GameMenu.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEGAME_ONLINE_API UGameMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	void Setup();

protected:
	virtual bool Initialize() override;

private:
	UFUNCTION()
	void BackToGame();

	UFUNCTION()
	void QuitGame();

	UPROPERTY(meta = (BindWidget))
	UButton* BackButton;

	UPROPERTY(meta = (BindWidget))
	UButton* QuitButton;
};
