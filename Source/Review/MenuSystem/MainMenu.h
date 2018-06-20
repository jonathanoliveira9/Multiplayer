// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "MainMenu.generated.h"


/** 
 * 
 */
UCLASS()
class REVIEW_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()

public:
	UMainMenu(const FObjectInitializer & FObjectInitializer); 

	void SetServerList(TArray<FString> ServerNames);

	void SelectIndex(uint32 Index);
protected:
	virtual bool Initialize();


private: 
	TSubclassOf <class UUserWidget> ServerRowClass;

	UPROPERTY(meta = (BindWidget))
		 class UButton*HostButton;

	UPROPERTY(meta = (BindWidget))
		class UButton*VideoButton;

	UPROPERTY(meta = (BindWidget))
		class UButton*VideoBackButton;

	UPROPERTY(meta = (BindWidget))
		class UButton*JoinButton;

	UPROPERTY(meta = (BindWidget))
		class UButton*QuitButton;
	
	UPROPERTY(meta = (BindWidget))
		class UButton*CancelJoinMenuButton;

	UPROPERTY(meta = (BindWidget))
		class UButton*ConfirmJoinMenuButton;

	UPROPERTY(meta = (BindWidget))
		class UWidgetSwitcher*MenuSwitcher;
	
	UPROPERTY(meta = (BindWidget))
		 class UWidget*JoinMenu;

	UPROPERTY(meta = (BindWidget))
		class UWidget*VideoMenu;

	UPROPERTY(meta = (BindWidget))
	class UWidget*MainMenu; 
	  
	UPROPERTY(meta = (BindWidget))
		class UPanelWidget*ServerList;


	UFUNCTION()
	 void HostServer();

	UFUNCTION()
		void JoinServer();

	UFUNCTION()
		void OpenJoinMenu(); 
	
	UFUNCTION()
	void OpenMainMenu();


	UFUNCTION()
	void QuitPressed();
	
	UFUNCTION()
		void OpenVideoMenu();

	UFUNCTION()
		void OpenMenuVideo();
		

	TOptional<uint32> SelectedIndex;
	void UpdateChildren(); 
};