// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UFO_WHEREISMYFOOD_API UGrabber : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Called for grab input

	UFUNCTION(BlueprintCallable)
	void Grab();

	UFUNCTION(BlueprintCallable)
	void Released();

private:

	UPROPERTY(EditAnywhere)
	float MaxGrabDistance = 600.0f;

	UPROPERTY(EditAnywhere)
	float GrabRadius = 100.0f;

	UPROPERTY(EditAnywhere)
	float HoldDistance = 300.0f;

	UPhysicsHandleComponent* GetPhysicsHandle() const;

	bool GetGrabbableInreach(FHitResult& OutHitResult) const;
		
};
