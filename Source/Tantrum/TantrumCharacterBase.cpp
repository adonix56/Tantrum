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

	PickupTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("PickupTrigger"));
	PickupTrigger->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATantrumCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	PickupTrigger->OnComponentBeginOverlap.AddDynamic(this, &ATantrumCharacterBase::OnPickupTriggerOverlapBegin);
	PickupTrigger->OnComponentEndOverlap.AddDynamic(this, &ATantrumCharacterBase::OnPickupTriggerOverlapEnd);
}

void ATantrumCharacterBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	PickupTrigger->OnComponentBeginOverlap.RemoveDynamic(this, &ATantrumCharacterBase::OnPickupTriggerOverlapBegin);
	PickupTrigger->OnComponentEndOverlap.RemoveDynamic(this, &ATantrumCharacterBase::OnPickupTriggerOverlapEnd);
	Super::EndPlay(EndPlayReason);
}

void ATantrumCharacterBase::OnPickupTriggerOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == nullptr)
		return;

	AThrowableActor* ThrowableActor = Cast<AThrowableActor>(OtherActor);
	if (ThrowableActor) {
		ThrowableObjects.Add(ThrowableActor);
	}
}

void ATantrumCharacterBase::OnPickupTriggerOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == nullptr)
		return;

	AThrowableActor* ThrowableActor = Cast<AThrowableActor>(OtherActor);
	if (ThrowableActor) {
		ThrowableObjects.RemoveSingleSwap(ThrowableActor);
	}
}

bool ATantrumCharacterBase::IsActorInViewRange(AActor* Target) {
	FVector Direction = Target->GetActorLocation() - GetActorLocation();
	Direction.Z = 0.0f;
	float angle = acos(Direction.GetSafeNormal().Dot(GetActorForwardVector())) * 180 / PI;
	return angle < MaxAngleToPull;
}

AThrowableActor* ATantrumCharacterBase::GetClosestThrowableObject()
{
	AThrowableActor* Closest = nullptr;
	float ShortestDistance = MAX_flt;
	for (int i = 0; i < ThrowableObjects.Num(); i++) {
		if (IsActorInViewRange(ThrowableObjects[i])) {
			float NewDistance = GetSquaredDistanceTo(ThrowableObjects[i]);
			if (NewDistance < ShortestDistance) {
				ShortestDistance = NewDistance;
				Closest = ThrowableObjects[i];
			}
		}
	}
	return Closest;
}

// Called every frame
void ATantrumCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (State == ECharacterThrowState::None) {
		AThrowableActor* Closest = GetClosestThrowableObject();
		if (Closest != CurrentThrowableObject) {
			if (CurrentThrowableObject)
				CurrentThrowableObject->SetHighlight(false);
			CurrentThrowableObject = Closest;
			if (CurrentThrowableObject)
				CurrentThrowableObject->SetHighlight(true);
		}
	}
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
		CurrentThrowableObject = GetClosestThrowableObject();
		//IsActorInViewRange(CurrentThrowableObject);
		if (CurrentThrowableObject && CurrentThrowableObject->PullToActor(this)) {
			State = ECharacterThrowState::RequestingPull;
			PlayPullMontage(TEXT("Start"));
			CurrentThrowableObject->SetHighlight(false);
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Got nothing to pull"));
	}
}

void ATantrumCharacterBase::PlayPullMontage(FName Section) {
	if (!PullAnimMontage) return;
	const float PlayRate = 1.0f;
	bool bPlayedSuccessfully = PlayAnimMontage(PullAnimMontage, PlayRate, Section) > 0.0f;
}

void ATantrumCharacterBase::RequestThrow()
{
	if (State == ECharacterThrowState::Holding) {
		PlayThrowMontage();
	}
}

void ATantrumCharacterBase::PlayThrowMontage() {
	if (!ThrowAnimMontage) return;
	const float PlayRate = 1.0f;
	bool bPlayedSuccessfully = PlayAnimMontage(ThrowAnimMontage, PlayRate) > 0.0f;
	if (bPlayedSuccessfully) {
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		
		if (!MontageEndedDelegate.IsBound()) {
			MontageEndedDelegate.BindUObject(this, &ATantrumCharacterBase::OnMontageEnded);
		}
		AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, ThrowAnimMontage);

		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &ATantrumCharacterBase::OnThrowNotify);
	}
}

void ATantrumCharacterBase::OnThrowNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload) {
	UE_LOG(LogTemp, Warning, TEXT("THROW!!"));
	if (State == ECharacterThrowState::Holding) {
		State = ECharacterThrowState::None;
		MoveIgnoreActorRemove(Cast<AActor>(CurrentThrowableObject));
		CurrentThrowableObject->Throw(GetActorForwardVector());
	}
}

void ATantrumCharacterBase::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted) {
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance()) {
		AnimInstance->OnPlayMontageNotifyBegin.RemoveDynamic(this, &ATantrumCharacterBase::OnThrowNotify);
	}
}

void ATantrumCharacterBase::Pickup(AActor* TargetObject)
{
	if (State == ECharacterThrowState::RequestingPull) {
		PlayPullMontage(TEXT("End"));
		State = ECharacterThrowState::Holding;
		MoveIgnoreActorAdd(TargetObject);
		ensure(TargetObject->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_r_ThrowableSocket")));
	}
}

