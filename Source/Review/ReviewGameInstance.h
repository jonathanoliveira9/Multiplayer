// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameEngine.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionInterface.h"

#include "MenuSystem/MenuInterface.h"
#include "ReviewGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class REVIEW_API UReviewGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()
public:
	UReviewGameInstance(const FObjectInitializer & FObjectInitializer);
	virtual void Init();
	
	UFUNCTION(BlueprintCallable)
		void LoadMenuWidget();

	UFUNCTION(BlueprintCallable)
		void InGameLoadMenu();

	UFUNCTION(exec)
		void Host(FString ServerName) override;

	UFUNCTION(exec)
		void Join(uint32 Index)override;

	virtual void LoadMainMenu() override;
	void RefreshServerList() override;

private:
	TSubclassOf<class  UUserWidget> MenuClass;
	TSubclassOf<class  UUserWidget> InGameMenuClass;

	class UMainMenu* Menu;   
	IOnlineSessionPtr SessionInterface;
	TSharedPtr<class FOnlineSessionSearch>SessionSearch;

	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void OnFindSessionComplete(bool Success);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	FString DesiredServerName;
	void CreateSession();
};
