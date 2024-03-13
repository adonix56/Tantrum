// Fill out your copyright notice in the Description page of Project Settings.


#include "TantrumGameModeBase.h"
#include "TantrumGameWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

ATantrumGameModeBase::ATantrumGameModeBase()
{
}

void ATantrumGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	CurrentGameState = EGameState::Waiting;
	DisplayCountdown();
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ATantrumGameModeBase::StartGame, GameCountdownDuration, false);
}

void ATantrumGameModeBase::PlayerReachedEnd()
{
	CurrentGameState = EGameState::GameOver;

	//TODO: Update Widget
	//TODO: Return to control PlayerController Input state
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
}
