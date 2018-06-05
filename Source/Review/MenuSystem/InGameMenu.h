// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "InGameMenu.generated.h"

/**
 * 
 */
UCLASS()
class REVIEW_API UInGameMenu : public UMenuWidget
{
	GENERATED_BODY()
protected:
	virtual bool Initialize();
	
private:
	UPROPERTY(meta = (BindWidget))
		class UButton*CancelButton;

	UPROPERTY(meta = (BindWidget))
		class UButton*QuitButton;
	UPROPERTY(meta = (BindWidget))
		class UButton*VideoBackButton;
	
	UPROPERTY(meta = (BindWidget))
		class UButton*OptionPauseButton;
	UPROPERTY(meta = (BindWidget))
		class UWidgetSwitcher*GameSwitcher;
	UPROPERTY(meta = (BindWidget))
		class UWidget*VideoMenu;

	UPROPERTY(meta = (BindWiget))
		class UWidget * InGameMenu;

	UFUNCTION()
		void CancelPressed();

	UFUNCTION()
		void QuitPressed();

	UFUNCTION()
	void OpenVideo();

	UFUNCTION()
		void BackToMenuPause();

};
