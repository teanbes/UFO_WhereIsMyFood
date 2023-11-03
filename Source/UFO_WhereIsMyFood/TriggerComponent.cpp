// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PigActor.h"


UTriggerComponent::UTriggerComponent():
bIsGameOver(false)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();

	// Delay to find all APigActors
	FTimerHandle DelayHandle;
	GetWorld()->GetTimerManager().SetTimer(DelayHandle, this, &UTriggerComponent::GetPigActors, 1.0f, false);
}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TArray<AActor*> Actors;
	GetOverlappingActors(Actors);

	int32 AmountOfActors = GetTriggerActor();
	if (AmountOfActors > 0)
	{
		UE_LOG(LogTemp, Display, TEXT("Trigger Actor found: %d"), AmountOfTriggers);

	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("no trigger actor"))
	}
}

int32 UTriggerComponent::GetTriggerActor()
{
	AmountOfTriggers = 0;

	TArray<AActor*> Actors;
	GetOverlappingActors(Actors);

	for (AActor* Actor : Actors)
	{
		if ( Actor->IsA(APigActor::StaticClass()))
		{
			AmountOfTriggers++;
			CheckForGameOver();
		}
	}

	//for (AActor* Actor : Actors)
	//{
	//	if (Actor->ActorHasTag(TriggerActorTag))
	//	{
	//		AmountOfTriggers++;
	//	}
	//}
	return AmountOfTriggers;
}

void UTriggerComponent::GetPigActors()
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APigActor::StaticClass(), FoundActors);
}

void UTriggerComponent::CheckForGameOver()
{

	if (AmountOfTriggers == FoundActors.Num())
	{
		bIsGameOver = true;
	}
}




