// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractInterface.h"
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
class TANTRUM_API ATantrumCharacterBase : public ACharacter, public IInteractInterface
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

	UPROPERTY(EditAnywhere, Category = "Speed")
	float BaseMoveSpeed = 300.0f;

	UPROPERTY(VisibleAnywhere, Category = "Speed")
	float SprintBonus = 500.0f;

	bool bIsSprinting = false;

	UPROPERTY(VisibleAnywhere, Category = "Speed")
	float CurrentMoveSpeedMultiplier = 1.0f;

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

	bool IsActorInViewRange(AActor* Target);

	UPROPERTY(EditAnywhere, Category = "Throwing")
	float MaxAngleToPull = 25.0f;

	void PlayPullMontage(FName Section);

	UPROPERTY(EditAnywhere, Category = "Throwing")
	UAnimMontage* PullAnimMontage = nullptr;

	void PlayThrowMontage();

	UPROPERTY(EditAnywhere, Category = "Throwing")
	UAnimMontage* ThrowAnimMontage = nullptr;

	UFUNCTION()
	void OnThrowNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	FOnMontageEnded MontageEndedDelegate;

	void ApplyEffect_Implementation(EEffectType EffectType, bool bIsBuff) override;

	void EndEffect();

	bool IsUnderEffect() const { return CurrentEffect != EEffectType::None; }
	bool bIsEffectBuff = false;

	float DefaultEffectCooldown = 5.0f;
	float EffectCooldown = 0.0f;

	EEffectType CurrentEffect = EEffectType::None;

	void ResetThrowableObject();

	void ChangeMoveSpeed();

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

	void RequestUseObject();

	void RequestSprint(bool Sprint);


	UFUNCTION()
	void Pickup(AActor* TargetObject);

};
