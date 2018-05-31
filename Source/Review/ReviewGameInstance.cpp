// Fill out your copyright notice in the Description page of Project Settings.

#include "ReviewGameInstance.h"
#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "MenuSystem/MainMenu.h" 
#include "MenuSystem/MenuWidget.h" 


UReviewGameInstance::UReviewGameInstance(const FObjectInitializer & FObjectInitializer) {
	
	ConstructorHelpers::FClassFinder <UUserWidget> MenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	if (!ensure(MenuBPClass.Class != nullptr)) return;
	
	MenuClass = MenuBPClass.Class;
	
	ConstructorHelpers::FClassFinder <UUserWidget> InGameMenuBPClass(TEXT("/Game/MenuSystem/WBP_InGameMenu"));
	if (!ensure(InGameMenuBPClass.Class != nullptr)) return;
	
	InGameMenuClass = InGameMenuBPClass.Class;
	
	UE_LOG(LogTemp, Warning, TEXT("GameInstance Constructor"));

}


void UReviewGameInstance:: Init() 
{

	UE_LOG(LogTemp, Warning, TEXT("GameInstance  Init"));
	UE_LOG(LogTemp, Warning, TEXT("Found class %s"), *MenuClass ->GetName());

}
void UReviewGameInstance::LoadMenu() {
	if (!ensure(MenuClass != nullptr)) return;
	Menu = CreateWidget<UMainMenu>(this, MenuClass);
	if (!ensure(Menu != nullptr)) return;
	Menu->AddToViewport();


	APlayerController * PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr))return;

	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(Menu->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = true;

	Menu->SetMenuInterface(this);

}

void UReviewGameInstance::InGameLoadMenu() {
	if (!ensure(InGameMenuClass != nullptr)) return;
	
	UMenuWidget* Menu = CreateWidget<UMenuWidget>(this, InGameMenuClass);
	
	if (!ensure(Menu != nullptr)) return;
	
	Menu->Setup();
	
	Menu->SetMenuInterface(this);

}

void UReviewGameInstance::Host() {
	if (Menu != nullptr)
	{
		Menu->TearDown();

	}

	UEngine*Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 2, FColor::Red , TEXT("Hosting"));

	//Travel to World
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;
	World->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");

}

void UReviewGameInstance::Join(const FString& Address) 
{

	if (Menu != nullptr)
	{
		Menu->TearDown();

	}

	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 2, FColor::Red, FString::Printf(TEXT("Joining %s"), *Address));

	APlayerController * PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr))return;
	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

 void UReviewGameInstance::LoadMainMenu()
 {
	 APlayerController* PlayerController = GetFirstLocalPlayerController();
	 if (!ensure(PlayerController != nullptr)) return;

	 PlayerController->ClientTravel("/Game/MenuSystem/MainMenu", ETravelType::TRAVEL_Absolute);
 
 }
