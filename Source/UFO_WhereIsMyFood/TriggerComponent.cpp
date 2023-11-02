// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"


UTriggerComponent::UTriggerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TArray<AActor*> Actors;
	GetOverlappingActors(Actors);

	//if (Actors.Num() > 0)
	//{
	//	FString ActorName = Actors[0]->GetActorNameOrLabel();
	//	UE_LOG(LogTemp, Display, TEXT("overlaping: %s"), *ActorName);
	//}

	//AActor* Actor = GetTriggerActor();
	int32 AmountOfActors = GetTriggerActor();
	if (AmountOfActors > 0)
	{
		
		UE_LOG(LogTemp, Display, TEXT("Trigger Actor found: %d"), AmountOfTriggers);
		//UE_LOG(LogTemp, Display, TEXT("Trigger Actor found" ))
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
		if (Actor->ActorHasTag(TriggerActorTag))
		{
			AmountOfTriggers++;
		}
	}
	return AmountOfTriggers;
}



//AActor* UTriggerComponent::GetTriggerActor() const
//{
//	TArray<AActor*> Actors;
//	GetOverlappingActors(Actors);
//	for (AActor* Actor : Actors)
//	{
//		
//		if (Actor->ActorHasTag(TriggerActorTag))
//		{
//			return Actor;
//		}
//	}
//	return nullptr;
//}
