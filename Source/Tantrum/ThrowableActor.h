// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractInterface.h"
#include "ThrowableActor.generated.h"

class UStaticMeshComponent;
class UProjectileMovementComponent;

UENUM(BlueprintType)
enum class EThrowableState : uint8 {
	Idle,
	Pull,
	Attach,
	Throw,
	Dropped
};

UCLASS()
class TANTRUM_API AThrowableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AThrowableActor();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	UFUNCTION()
	void AttachToPlayer(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere)
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(VisibleAnywhere)
	EThrowableState State = EThrowableState::Idle;

	UPROPERTY(VisibleAnywhere)
	AActor* PullActor = nullptr;

	UPROPERTY(EditAnywhere)
	EEffectType EffectType = EEffectType::None;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	bool IsIdle() const { return State == EThrowableState::Idle; }

	EEffectType const GetEffectType() { return EffectType; }

	UFUNCTION(BlueprintCallable)
	void Throw(FVector Forward, float Power = 1000.0f);

	UFUNCTION(BlueprintCallable)
	bool PullToActor(AActor* Target);

	UFUNCTION()
	void Launch(FVector Velocity);

	UFUNCTION()
	void SetHighlight(bool Activate);

};
