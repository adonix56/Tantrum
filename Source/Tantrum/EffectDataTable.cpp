// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectDataTable.h"

// Sets default values
AEffectDataTable::AEffectDataTable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEffectDataTable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEffectDataTable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FEffectStats* AEffectDataTable::GetEffectStats(EEffectType EffectType)
{
	if (EffectsTable) {
		static const FString ContextString(TEXT("Effect Context"));
		FEffectStats* EffectStatRow = EffectsTable->FindRow<FEffectStats>(FName(UEnum::GetDisplayValueAsText(EffectType).ToString()), ContextString, true);
		return EffectStatRow;
	}
	return nullptr;
}

