// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

#include "ServerRowWidget.generated.h"

class UMainMenu;

/**
 * 
 */
UCLASS()
class PUZZLEGAME_ONLINE_API UServerRowWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ServerName;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ConnectionFraction;

	UPROPERTY(meta = (BindWidget))
	UButton* BackgroundButton;

	UPROPERTY(BlueprintReadOnly)
	bool bSelected = false;

	void Setup(UMainMenu* InParent, uint32 InIndex);

protected:
	virtual bool Initialize() override;

private:
	UPROPERTY()
	UMainMenu* Parent = nullptr;

	uint32 Index;

	UFUNCTION()
	void ClickBackgroundButton();
};
