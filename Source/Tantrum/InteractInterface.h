// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractInterface : public UInterface
{
	GENERATED_BODY()
};

UENUM(BlueprintType)
enum class EEffectType : uint8 {
	None	UMETA(DisplayName = "None"),
	Speed	UMETA(DisplayName = "Speed"),
	Jump	UMETA(DisplayName = "Jump"),
	Power	UMETA(DisplayName = "Power"),
};

class TANTRUM_API IInteractInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/*
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interact")
	const bool TypedInteract();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interact")
	void NonTypedInteract();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interact")
	void SignatureInteract(bool& Return);
	*/

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
	void ApplyEffect(EEffectType EffectType, bool bIsBuff);

};
