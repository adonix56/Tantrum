// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TantrumCharacterBase.generated.h"

class AThrowableActor;
class USphereComponent;

UENUM(BlueprintType)
enum class ECharacterThrowState : uint8 {
	None UMETA(DisplayName = "None"),
	RequestingPull UMETA(DisplayName = "RequestingPull"),
	Holding UMETA(DisplayName = "Holding"),
	RequestingThrow UMETA(DisplayName = "RequestingThrow"),
	Throwing UMETA(DisplayName = "Throwing"),
};

UCLASS()
class TANTRUM_API ATantrumCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATantrumCharacterBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup")
	USphereComponent* PickupTrigger = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void OnPickupTriggerOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnPickupTriggerOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, Category = "Fall Impact")
	float MinImpactSpeed = 800.0f;

	UPROPERTY(EditAnywhere, Category = "Fall Impact")
	float MaxImpactSpeed = 1600.0f;

	UPROPERTY(VisibleAnywhere, Category = "Throwing")
	TArray<AThrowableActor*> ThrowableObjects;

	AThrowableActor* GetClosestThrowableObject();

	UPROPERTY(EditAnywhere, Category = "Throwing")
	AThrowableActor* CurrentThrowableObject;

	UPROPERTY(VisibleAnywhere)
	ECharacterThrowState State = ECharacterThrowState::None;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Landed(const FHitResult& Hit) override;

	UFUNCTION(BlueprintPure)
	const bool IsPullingObject() { return State == ECharacterThrowState::RequestingPull; }

	UFUNCTION()
	void RequestPull();

	UFUNCTION()
	void RequestThrow();

	UFUNCTION()
	void Pickup(AActor* TargetObject);

};
