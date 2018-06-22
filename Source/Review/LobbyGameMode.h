// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ReviewGameMode.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class REVIEW_API ALobbyGameMode : public AReviewGameMode
{
	GENERATED_BODY()
public:
	 void PostLogin(APlayerController* NewPlayer) override;
	 void Logout(AController*Exiting) override;

private:
	uint32 NumberOfPlayers = 0;

};
