// Fill out your copyright notice in the Description page of Project Settings.


#include "TantrumLevelEndTrigger.h"
#include "TantrumGameModeBase.h"
#include "TantrumCharacterBase.h"

ATantrumLevelEndTrigger::ATantrumLevelEndTrigger()
{
	OnActorBeginOverlap.AddDynamic(this, &ATantrumLevelEndTrigger::OnOverlapBegin);
}

void ATantrumLevelEndTrigger::BeginPlay()
{
	Super::BeginPlay();
	GameModeRef = GetWorld()->GetAuthGameMode<ATantrumGameModeBase>();
}

void ATantrumLevelEndTrigger::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor == Cast<ATantrumCharacterBase>(OtherActor)) {
		GameModeRef->PlayerReachedEnd();
	}
}
