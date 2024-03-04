// Fill out your copyright notice in the Description page of Project Settings.


#include "ThrowableActor.h"

#include "TantrumCharacterBase.h"
#include "Kismet/GameplayStatics.h"

#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/EngineTypes.h"
#include "TantrumCharacterBase.h"

// Sets default values
AThrowableActor::AThrowableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	RootComponent = StaticMeshComponent;

}

// Called when the game starts or when spawned
void AThrowableActor::BeginPlay()
{
	Super::BeginPlay();
	//ProjectileMovementComponent->OnProjectileStop.AddDynamic(this, &AThrowableActor::ProjectileStop);
	OnActorHit.AddDynamic(this, &AThrowableActor::AttachToPlayer);
}

void AThrowableActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	//ProjectileMovementComponent->OnProjectileStop.RemoveDynamic(this, &AThrowableActor::ProjectileStop);
	Super::EndPlay(EndPlayReason);
}

void AThrowableActor::NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) 
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);


}

bool AThrowableActor::PullToActor(AActor* Target)
{
	if (State == EThrowableState::Idle || State == EThrowableState::Throw) {
		Launch(FVector::UpVector * 1000.0f);
		State = EThrowableState::Pull;
		ProjectileMovementComponent->Activate(true);
		ProjectileMovementComponent->bIsHomingProjectile = true;
		ProjectileMovementComponent->HomingTargetComponent = Target->GetComponentByClass<USceneComponent>();
		return true;
	}
	return false;
}

void AThrowableActor::AttachToPlayer(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	ATantrumCharacterBase* CharacterHit = Cast<ATantrumCharacterBase>(OtherActor);
	if (State == EThrowableState::Pull && CharacterHit) {
		State = EThrowableState::Attach;
		ProjectileMovementComponent->Deactivate();
		CharacterHit->Pickup(this);
	}
}

// Called every frame
void AThrowableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AThrowableActor::Throw(FVector Forward)
{
	if (State == EThrowableState::Attach) {
		State = EThrowableState::Throw;
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		Launch(Forward * 1000.0f);
	}
}

void AThrowableActor::Launch(FVector Velocity) {
	ProjectileMovementComponent->SetUpdatedComponent(RootComponent);
	ProjectileMovementComponent->Activate(true);
	ProjectileMovementComponent->bIsHomingProjectile = false;
	ProjectileMovementComponent->bInitialVelocityInLocalSpace = false;
	ProjectileMovementComponent->Velocity = Velocity;
}

void AThrowableActor::SetHighlight(bool Activate)
{
	StaticMeshComponent->SetRenderCustomDepth(Activate);
}

