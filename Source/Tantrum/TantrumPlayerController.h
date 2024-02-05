// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TantrumPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TANTRUM_API ATantrumPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	//void BeginPlay() override;
	void SetupInputComponent() override;
	void RequestJump();
	void RequestStopJump();
	void RequestCrouch();
	void RequestSprint();
	void RequestWalk();
	void RequestMoveForward(float AxisValue);
	void RequestMoveRight(float AxisValue);
	void RequestLookUp(float AxisValue);
	void RequestLookRight(float AxisValue);

public:

	UPROPERTY(EditAnywhere, Category="Look")
	float BaseLookUpRate = 90.0f;

	UPROPERTY(EditAnywhere, Category="Look")
	float BaseLookRightRate = 90.0f;
};
