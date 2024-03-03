// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TantrumCharacterBase.generated.h"

class AThrowableActor;
class USphereComponent;

UCLASS()
class TANTRUM_API ATantrumCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATantrumCharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup")
	USphereComponent* PickupTrigger;

	UPROPERTY(EditAnywhere, Category = "Fall Impact")
	float MinImpactSpeed = 800.0f;

	UPROPERTY(EditAnywhere, Category = "Fall Impact")
	float MaxImpactSpeed = 1600.0f;

	enum class ECharacterThrowState {
		None,
		RequestingPull,
		Holding,
		RequestingThrow,
		Throwing,
	};

	UPROPERTY(VisibleAnywhere, Category = "Throwing")
	TArray<AThrowableActor*> ThrowableObjects;

	AThrowableActor* GetClosestThrowableObject();

	AThrowableActor* CurrentThrowableObject;

	ECharacterThrowState State = ECharacterThrowState::None;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Landed(const FHitResult& Hit) override;

	UFUNCTION()
	void RequestPull();

	UFUNCTION()
	void RequestThrow();

	UFUNCTION()
	void Pickup(AActor* TargetObject);

};
