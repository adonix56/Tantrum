// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TantrumGameModeBase.generated.h"


UENUM(BlueprintType)
enum class EGameState : uint8 {
	None UMETA(DisplayName = "None"),
	Waiting UMETA(DisplayName = "Waiting"),
	Playing UMETA(DisplayName = "Playing"),
	Paused UMETA(DisplayName = "Paused"),
	GameOver UMETA(DisplayName = "GameOver"),
};
/**
 * 
 */
UCLASS()
class TANTRUM_API ATantrumGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	ATantrumGameModeBase();

	virtual void BeginPlay() override;

	const EGameState GetCurrentGameState() { return CurrentGameState; };

	void PlayerReachedEnd();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="States", meta=(AllowPrivateAccess="true"))
	EGameState CurrentGameState = EGameState::None;

	UPROPERTY(EditAnywhere, Category="Game Details")
	float GameCountdownDuration = 4.0f;

	FTimerHandle TimerHandle;

	void DisplayCountdown();
	void StartGame();
	
};
