// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TantrumGameModeBase.generated.h"

class UTantrumGameWidget;

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

	UFUNCTION(BlueprintCallable)
	EGameState GetCurrentGameState() const { return CurrentGameState; };

	void PlayerReachedEnd();

protected:

	UPROPERTY(VisibleAnywhere, Category="States")
	EGameState CurrentGameState = EGameState::None;

	UPROPERTY(EditAnywhere, Category="Game Details")
	float GameCountdownDuration = 4.0f;

	FTimerHandle TimerHandle;

	UPROPERTY()
	UTantrumGameWidget* GameWidget;
	UPROPERTY(EditAnywhere, Category="Widget")
	TSubclassOf<UTantrumGameWidget> GameWidgetClass;

	APlayerController* PC = nullptr;

	void DisplayCountdown();
	void StartGame();
	
};
