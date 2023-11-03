// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "TriggerComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UFO_WHEREISMYFOOD_API UTriggerComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTriggerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	UPROPERTY(EditAnywhere)
	FName TriggerActorTag;

	//AActor* GetTriggerActor() const;

	int32 GetTriggerActor();
	
	int32 AmountOfTriggers;

	TArray<AActor*> FoundActors;

	void GetPigActors();

	UFUNCTION(BlueprintCallable)
	void CheckForGameOver();

	bool bIsGameOver;

public:

	UFUNCTION(BlueprintCallable)
	int32 GetAmountOfTriggers() const { return AmountOfTriggers; }

	UFUNCTION(BlueprintCallable)
	int32 GetAmountOfPigActors() const { return FoundActors.Num(); }

	UFUNCTION(BlueprintCallable)
	bool IsGameOver() const { return bIsGameOver;  }



};
