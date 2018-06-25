// Fill out your copyright notice in the Description page of Project Settings.

#include "ReviewGameInstance.h"

#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

#include "OnlineSessionSettings.h"


#include "MenuSystem/MainMenu.h" 
#include "MenuSystem/MenuWidget.h" 

const static FName SESSION_NAME =  TEXT("Game");
const static FName SERVER_NAME_SETTINGS_KEY = TEXT("ServerName");

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
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem != nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Found subsystem %s"), *Subsystem->GetSubsystemName().ToString());
SessionInterface =	Subsystem->GetSessionInterface();
	if (SessionInterface.IsValid()) {
		SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UReviewGameInstance::OnCreateSessionComplete);
		SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UReviewGameInstance::OnDestroySessionComplete);
		SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UReviewGameInstance::OnFindSessionComplete);
		SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UReviewGameInstance::OnJoinSessionComplete);

	}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Found no subsystem"));

	}

}
void UReviewGameInstance::LoadMenuWidget() {
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

void UReviewGameInstance::Host(FString ServerName) 
{
	DesiredServerName = ServerName;

	if (SessionInterface.IsValid())
	{
		auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
		if (ExistingSession != nullptr) {
			SessionInterface->DestroySession(SESSION_NAME);

		}
		else
		{
			CreateSession();
    
		}}
	
}
void UReviewGameInstance::OnDestroySessionComplete(FName SessionName, bool Success) {
	if (Success) {
		CreateSession();
	}

}
void UReviewGameInstance::CreateSession() 
{
	if (SessionInterface.IsValid()) {
		FOnlineSessionSettings SessionSettings;
		if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL") {
			SessionSettings.bIsLANMatch = true;
		}
		else {
			SessionSettings.bIsLANMatch = false;
		}
	//	SessionSettings.bIsLANMatch = true;
		SessionSettings.NumPublicConnections = 5;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.bUsesPresence = true;
		SessionSettings.Set(SERVER_NAME_SETTINGS_KEY, DesiredServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
	}
}

void UReviewGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)

{
	if (!Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not create session"));
		return;
		 
	}
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
	World->ServerTravel("/Game/BP_Review/Maps/Lobby?listen");
	

}

void UReviewGameInstance::RefreshServerList() 
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());

	if (SessionSearch.IsValid())   
	{
		//SessionSearch -> bIsLanQuery= true;

		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		UE_LOG(LogTemp, Warning, TEXT("Starting Find Session"));
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

void UReviewGameInstance::OnFindSessionComplete(bool Success)
{
	if (Success && SessionSearch.IsValid() && Menu != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Finished Find Session"));

		TArray<FServerData> ServerNames;
		for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
		{
			UE_LOG(LogTemp, Warning, TEXT("Found session names: %s"), *SearchResult.GetSessionIdStr());
			FServerData Data;
			Data.MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
			Data.CurrentPlayers = Data.MaxPlayers - SearchResult.Session.NumOpenPublicConnections;
			Data.HostUsername = SearchResult.Session.OwningUserName;
			FString ServerName;
			if (SearchResult.Session.SessionSettings.Get(SERVER_NAME_SETTINGS_KEY, ServerName))
			{
				Data.Name = ServerName;
			}
			else
			{
				Data.Name = "Could not find name.";
			}
			ServerNames.Add(Data);
		}

		Menu->SetServerList(ServerNames);
	}

}

void UReviewGameInstance::Join(uint32 Index) 
{
	if (!SessionInterface.IsValid()) return;
	if (!SessionSearch.IsValid()) return;
	if (Menu != nullptr)
	{
		Menu->TearDown();
	}
	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);



}
void UReviewGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (!SessionInterface.IsValid()) return;
	 
	FString Address;
	if (!SessionInterface->GetResolvedConnectString(SessionName, Address)) {
		UE_LOG(LogTemp, Warning, TEXT("Could not get connect string"));
		return;
}

	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;
	
		Engine->AddOnScreenDebugMessage(0, 2, FColor::Red, FString::Printf(TEXT("Joining %s"), *Address));
	
		APlayerController * PlayerController = GetFirstLocalPlayerController();
		if (!ensure(PlayerController != nullptr))return;
		PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}


void UReviewGameInstance::StartSession()
 {
	if (SessionInterface.IsValid())
		 {
		SessionInterface->StartSession(SESSION_NAME);
		}
	}
 void UReviewGameInstance::LoadMainMenu()
 {
	 APlayerController* PlayerController = GetFirstLocalPlayerController();
	 if (!ensure(PlayerController != nullptr)) return;

	 PlayerController->ClientTravel("/Game/MenuSystem/MainMenu", ETravelType::TRAVEL_Absolute);
 
 }
