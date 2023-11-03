// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TriggerComponent.h"
#include "UFOPawn.generated.h"

UCLASS()
class UFO_WHEREISMYFOOD_API AUFOPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AUFOPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
	class UPhysicsThrusterComponent* PhysicsThruster;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UFO Movement", meta = (AllowPrivateAccess = "true"), meta = (ClampMin = -60, ClampMax = 1500))
	float VariableUpForce;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UFO Movement", meta = (AllowPrivateAccess = "true"))
	float ConstantUpForce;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UFO Movement", meta = (AllowPrivateAccess = "true"))
	float YawRotationSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UFO Movement", meta = (AllowPrivateAccess = "true"))
	float DesiredTiltAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UFO Movement", meta = (AllowPrivateAccess = "true"), meta = (ClampMin = 0))
	float TiltSpeedClampBound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UFO Movement", meta = (AllowPrivateAccess = "true"), meta = (ClampMin = 0))
	float TiltingSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UFO Movement", meta = (AllowPrivateAccess = "true"), meta = (ClampMin = 0))
	float MaxTiltAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effects, meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* AbductionRay;

public:

	// Input Binding functions

	UFUNCTION()
	void FlightRight(float Value);

	UFUNCTION()
	void FlightUp(float Value);

	void ResetTilt(float DeltaTime);
	
};
