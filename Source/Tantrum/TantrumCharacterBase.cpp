// Fill out your copyright notice in the Description page of Project Settings.


#include "TantrumCharacterBase.h"
#include "TantrumPlayerController.h"
#include "ThrowableActor.h"
#include "Engine/EngineTypes.h"
#include "Components/SphereComponent.h"

// Sets default values
ATantrumCharacterBase::ATantrumCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PickupTrigger->CreateDefaultSubobject<USphereComponent>(TEXT("PickupTrigger"));
	PickupTrigger->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATantrumCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void ATantrumCharacterBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

AThrowableActor* ATantrumCharacterBase::GetClosestThrowableObject()
{
	if (ThrowableObjects.Num() > 0) {

	}
	return nullptr;
}

// Called every frame
void ATantrumCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATantrumCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATantrumCharacterBase::Landed(const FHitResult& Hit) 
{
	Super::Landed(Hit);

	//Custom Landed Code
	ATantrumPlayerController* PlayerController = GetController<ATantrumPlayerController>();

	if (PlayerController) {
		const float LandingSpeed = FMath::Abs(GetVelocity().Z);
		if (LandingSpeed < MinImpactSpeed) { return; }

		const float DeltaImpact = MaxImpactSpeed - MinImpactSpeed;
		const float FallRatio = FMath::Clamp((LandingSpeed - MinImpactSpeed) / DeltaImpact, 0.0f, 1.0f);
		const bool bAffectSmall = FallRatio <= 0.5f;
		const bool bAffectLarge = FallRatio > 0.5f;
		//scale intensity by landing impact
		PlayerController->PlayDynamicForceFeedback(FallRatio, 0.5f, bAffectLarge, bAffectSmall, bAffectLarge, bAffectSmall);
	}
}

void ATantrumCharacterBase::RequestPull()
{
	if (State == ECharacterThrowState::None && ThrowableObjects.Num() > 0) { 
		State = ECharacterThrowState::RequestingPull;
		ThrowTestObject->PullToActor(this);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Got nothing to pull"));
	}
}

void ATantrumCharacterBase::RequestThrow()
{
	if (State == ECharacterThrowState::Holding) {
		State = ECharacterThrowState::None;
		MoveIgnoreActorRemove(Cast<AActor>(CurrentThrowableObject));
		CurrentThrowableObject->Throw(GetActorForwardVector());
	}
}

void ATantrumCharacterBase::Pickup(AActor* TargetObject)
{
	if (State == ECharacterThrowState::RequestingPull) {
		State = ECharacterThrowState::Holding;
		MoveIgnoreActorAdd(TargetObject);
		ensure(TargetObject->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_l_ThrowableSocket")));
	}
}

