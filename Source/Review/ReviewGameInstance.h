// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameEngine.h"
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
		void LoadMenu();

	UFUNCTION(BlueprintCallable)
		void InGameLoadMenu();

	UFUNCTION(exec)
		void Host() override;

	UFUNCTION(exec)
		void Join(const FString& Address)override;

	virtual void LoadMainMenu() override;


private:
	TSubclassOf<class  UUserWidget> MenuClass;
	TSubclassOf<class  UUserWidget> InGameMenuClass;

	class UMainMenu* Menu;   


};