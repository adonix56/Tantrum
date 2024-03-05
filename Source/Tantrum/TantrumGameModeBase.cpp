// Fill out your copyright notice in the Description page of Project Settings.


#include "TantrumGameModeBase.h"

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
	//TODO: Return and create, store, and display widget
}

void ATantrumGameModeBase::StartGame()
{
	CurrentGameState = EGameState::Playing;
}
