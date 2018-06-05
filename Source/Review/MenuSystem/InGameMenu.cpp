// Fill out your copyright notice in the Description page of Project Settings.

#include "InGameMenu.h"
#include "Components/Button.h"
#include "MainMenu.h"
#include "Components/WidgetSwitcher.h"


bool UInGameMenu::Initialize()
{

	bool Success = Super::Initialize();
	if (!Success) return false;
	
	if (!ensure(CancelButton != nullptr))return false;
	CancelButton->OnClicked.AddDynamic(this, &UInGameMenu::CancelPressed);
 
	if (!ensure(QuitButton != nullptr))return false;
	QuitButton->OnClicked.AddDynamic(this, &UInGameMenu::QuitPressed);

	if (!ensure(OptionPauseButton != nullptr))return false;
    OptionPauseButton->OnClicked.AddDynamic(this, &UInGameMenu::OpenVideo);
	

	if (!ensure(VideoBackButton != nullptr))return false;
	VideoBackButton->OnClicked.AddDynamic(this, &UInGameMenu::BackToMenuPause);

		return true;
}
void UInGameMenu::CancelPressed()
{
	TearDown();

}

void UInGameMenu::QuitPressed()
{
	if (MenuInterface != nullptr){
		TearDown();
		MenuInterface->LoadMainMenu();
		
	}
}

void UInGameMenu::OpenVideo()
{
	if (!ensure(GameSwitcher != nullptr)) return;
	if (!ensure(OptionPauseButton != nullptr)) return;

	GameSwitcher->SetActiveWidget(VideoMenu);

}

void UInGameMenu::BackToMenuPause() {
	if (!ensure(GameSwitcher != nullptr)) return;
	if (!ensure(VideoBackButton != nullptr)) return;
	GameSwitcher->SetActiveWidget(InGameMenu);
 

}
