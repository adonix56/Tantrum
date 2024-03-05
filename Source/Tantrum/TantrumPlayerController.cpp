// Fill out your copyright notice in the Description page of Project Settings.


#include "TantrumPlayerController.h"
#include "TantrumCharacterBase.h"
#include "TantrumGameModeBase.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void ATantrumPlayerController::BeginPlay()
{
	Super::BeginPlay();
	GameModeRef = Cast<ATantrumGameModeBase>(GetWorld()->GetAuthGameMode());
}

void ATantrumPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();

	if (InputComponent) {
		InputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ATantrumPlayerController::RequestJump);
		InputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Released, this, &ATantrumPlayerController::RequestStopJump);
		InputComponent->BindAction(TEXT("Crouch"), EInputEvent::IE_Pressed, this, &ATantrumPlayerController::RequestCrouch);
		InputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Pressed, this, &ATantrumPlayerController::RequestSprint);
		InputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Released, this, &ATantrumPlayerController::RequestWalk);
		InputComponent->BindAction(TEXT("Pull"), EInputEvent::IE_Pressed, this, &ATantrumPlayerController::RequestPull);
		InputComponent->BindAction(TEXT("Throw"), EInputEvent::IE_Pressed, this, &ATantrumPlayerController::RequestThrow);
		InputComponent->BindAxis(TEXT("MoveForward"), this, &ATantrumPlayerController::RequestMoveForward);
		InputComponent->BindAxis(TEXT("MoveRight"), this, &ATantrumPlayerController::RequestMoveRight);
		InputComponent->BindAxis(TEXT("LookUp"), this, &ATantrumPlayerController::RequestLookUp);
		InputComponent->BindAxis(TEXT("LookRight"), this, &ATantrumPlayerController::RequestLookRight);
	}
}

void ATantrumPlayerController::RequestJump() {
	if (!GameModeRef || GameModeRef->GetCurrentGameState() != EGameState::Playing) { return; }
	if (GetCharacter()) {
		GetCharacter()->Jump();
	}
}

void ATantrumPlayerController::RequestStopJump() {
	if (GetCharacter()) {
		GetCharacter()->StopJumping();
	}
}

void ATantrumPlayerController::RequestCrouch() {
	if (!GameModeRef || GameModeRef->GetCurrentGameState() != EGameState::Playing) { return; }
	if (GetCharacter()) {
		if (!GetCharacter()->GetCharacterMovement()->IsMovingOnGround()) { return; }
		if (GetCharacter()->bIsCrouched)
			GetCharacter()->UnCrouch();
		else
			GetCharacter()->Crouch();
	}
}

void ATantrumPlayerController::RequestSprint() {
	if (!GameModeRef || GameModeRef->GetCurrentGameState() != EGameState::Playing) { return; }
	if (GetCharacter()) {
		GetCharacter()->GetCharacterMovement()->MaxWalkSpeed = 800.0f;
	}
}

void ATantrumPlayerController::RequestWalk() {
	if (!GameModeRef || GameModeRef->GetCurrentGameState() != EGameState::Playing) { return; }
	if (GetCharacter()) {
		GetCharacter()->GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	}
}

void ATantrumPlayerController::RequestPull() {
	if (!GameModeRef || GameModeRef->GetCurrentGameState() != EGameState::Playing) { return; }
	if (GetCharacter()) {
		ATantrumCharacterBase* CharacterBase = Cast<ATantrumCharacterBase>(GetCharacter());
		if (CharacterBase) {
			CharacterBase->RequestPull();
		}
		//GetCharacter()
	}
}

void ATantrumPlayerController::RequestThrow() {
	if (!GameModeRef || GameModeRef->GetCurrentGameState() != EGameState::Playing) { return; }
	if (GetCharacter()) {
		ATantrumCharacterBase* CharacterBase = Cast<ATantrumCharacterBase>(GetCharacter());
		if (CharacterBase) {
			CharacterBase->RequestThrow();
		}
	}
}

void ATantrumPlayerController::RequestMoveForward(float AxisValue) {
	if (!GameModeRef || GameModeRef->GetCurrentGameState() != EGameState::Playing) { return; }
	if (AxisValue != 0.0f) {
		FRotator const ControlSpaceRot = GetControlRotation();
		GetPawn()->AddMovementInput(FRotationMatrix(ControlSpaceRot).GetScaledAxis(EAxis::X), AxisValue);
	}
}

void ATantrumPlayerController::RequestMoveRight(float AxisValue) {
	if (!GameModeRef || GameModeRef->GetCurrentGameState() != EGameState::Playing) { return; }
	if (AxisValue != 0.0f) {
		FRotator const ControlSpaceRot = GetControlRotation();
		GetPawn()->AddMovementInput(FRotationMatrix(ControlSpaceRot).GetScaledAxis(EAxis::Y), AxisValue);
	}
}

void ATantrumPlayerController::RequestLookUp(float AxisValue) {
	AddPitchInput(AxisValue * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ATantrumPlayerController::RequestLookRight(float AxisValue) {
	AddYawInput(AxisValue * BaseLookRightRate * GetWorld()->GetDeltaSeconds());
}