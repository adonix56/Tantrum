// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "InteractInterface.h"
#include "EffectDataTable.generated.h"

USTRUCT(BlueprintType)
struct FEffectStats : public FTableRowBase {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EEffectType EffectType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float EffectStrengthBuff = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float EffectStrengthDebuff = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString EffectDescription = "";
};

UCLASS()
class TANTRUM_API AEffectDataTable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEffectDataTable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Effects")
	UDataTable* EffectsTable;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FEffectStats* GetEffectStats(EEffectType EffectType);
};
