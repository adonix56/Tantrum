// Fill out your copyright notice in the Description page of Project Settings.


#include "TantrumGameModeBase.h"
#include "TantrumGameWidget.h"
#include "Kismet/GameplayStatics.h"
//#include "Engine/GameInstance.h"
#include "Blueprint/UserWidget.h"

ATantrumGameModeBase::ATantrumGameModeBase()
{
}

void ATantrumGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	CurrentGameState = EGameState::Waiting; 
	bSplitScreen = GetWorld()->GetName().Compare(FString("SplitScreenMP")) == 0;
	SpawnPlayers();
	DisplayCountdown();
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ATantrumGameModeBase::StartGame, GameCountdownDuration, false);
}

void ATantrumGameModeBase::PlayerReachedEnd()
{
	CurrentGameState = EGameState::GameOver;

	GameWidget->LevelComplete();
	FInputModeUIOnly InputMode;
	PC->SetInputMode(InputMode);
	PC->SetShowMouseCursor(true);
}

void ATantrumGameModeBase::SpawnPlayers() {
	if (bSplitScreen) {
		FString Error;
		GetWorld()->GetGameInstance()->CreateLocalPlayer(-1, Error, true);
	}
}

void ATantrumGameModeBase::DisplayCountdown()
{
	if (!GameWidgetClass) { return; }

	PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	GameWidget = CreateWidget<UTantrumGameWidget>(PC, GameWidgetClass);
	GameWidget->AddToViewport();
	GameWidget->StartCountdown(GameCountdownDuration, this);
}

void ATantrumGameModeBase::StartGame()
{
	CurrentGameState = EGameState::Playing; 
	FInputModeGameOnly InputMode;
	PC->SetInputMode(InputMode);
	PC->SetShowMouseCursor(false);
}
