// Fill out your copyright notice in the Description page of Project Settings.

#include "ServerRowWidget.h"

#include "MainMenu.h"

void UServerRowWidget::Setup(UMainMenu* InParent, uint32 InIndex)
{
	Parent = InParent;
	Index = InIndex;
}

bool UServerRowWidget::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (bSuccess)
	{
		if (BackgroundButton)
		{
			BackgroundButton->OnClicked.AddDynamic(this, &UServerRowWidget::ClickBackgroundButton);
		}
	}
	return bSuccess;
}

void UServerRowWidget::ClickBackgroundButton()
{
	if (Parent) { Parent->SelectIndex(Index); }
}
