// Fill out your copyright notice in the Description page of Project Settings.

#include "UFOPawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "PhysicsEngine/PhysicsThrusterComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

// Sets default values
AUFOPawn::AUFOPawn() :
VariableUpForce(2000.0f),
ConstantUpForce(970.0f),
YawRotationSpeed(60.0f),
DesiredTiltAngle(30.0f),
TiltSpeedClampBound(20.0f),
TiltingSpeed(5.0f),
MaxTiltAngle(70.0f)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create Mesh component
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetSimulatePhysics(true);
	Mesh->SetLinearDamping(1);
	Mesh->SetAngularDamping(10);

	// Create Spring Arm Component
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->AttachToComponent(Mesh, FAttachmentTransformRules::KeepWorldTransform);
	SpringArm->SetRelativeRotation(FRotator(0, 0, -90));
	SpringArm->TargetArmLength = 2000.0f;
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = false;

	// Create Follow Camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->AttachToComponent(SpringArm, FAttachmentTransformRules::KeepWorldTransform);

	PhysicsThruster = CreateDefaultSubobject<UPhysicsThrusterComponent>(TEXT("Physics Thruster"));
	PhysicsThruster->AttachToComponent(Mesh, FAttachmentTransformRules::KeepWorldTransform);
	PhysicsThruster->SetRelativeRotation(FRotator(-90, 0, 0));
	PhysicsThruster->bAutoActivate = true;

	// Automatically possess pawn
	this->AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void AUFOPawn::BeginPlay()
{
	Super::BeginPlay();
	
	// Set proper center of mass between mesh object and physics thruster
	FVector ThrusterCenterOfMassDifference = PhysicsThruster->GetComponentLocation() - Mesh->GetCenterOfMass();
	FRotator InvertedMeshRotation = Mesh->GetComponentRotation().GetInverse();
	FVector CenterOfMassActualOffset = InvertedMeshRotation.RotateVector(ThrusterCenterOfMassDifference);
	FVector CenterOfMassWantedOffset = FVector(CenterOfMassActualOffset.X, CenterOfMassActualOffset.Y, 0);
	Mesh->SetCenterOfMass(CenterOfMassWantedOffset, FName());

}

// Called every frame
void AUFOPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Temporary to prevent z-axis rotation
	FRotator CurrentRotation = GetActorRotation();
	CurrentRotation.Yaw = 0.0f;
	SetActorRotation(CurrentRotation);

	// Reset the tilt if it exceeds the maximum angle
	ResetTilt(DeltaTime);

}

// Called to bind functionality to input
void AUFOPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Right", this, &AUFOPawn::FlightRight);
	PlayerInputComponent->BindAxis("Flight Up", this, &AUFOPawn::FlightUp);
}


void AUFOPawn::FlightRight(float Value)
{
	float DesiredAngle = Value * DesiredTiltAngle + Mesh->GetComponentRotation().Pitch;
	float ClampedValue = FMath::Clamp(DesiredAngle, - TiltSpeedClampBound, TiltSpeedClampBound);
	Mesh->AddTorqueInDegrees(GetActorRightVector() * ClampedValue * TiltingSpeed, FName(), true);
}

void AUFOPawn::FlightUp(float Value)
{
	float DesiredUpForce = Value * VariableUpForce;
	PhysicsThruster->ThrustStrength = DesiredUpForce / GetActorUpVector().Z * Mesh->GetMass();

	// Get the current location and zero out the Y-component
	FVector CurrentLocation = GetActorLocation();
	CurrentLocation.Y = 0.0f;
	SetActorLocation(CurrentLocation);
}

void AUFOPawn::ResetTilt(float DeltaTime)
{
	FRotator CurrentRotation = GetActorRotation();
	if (FMath::Abs(CurrentRotation.Pitch) > MaxTiltAngle)
	{
		CurrentRotation.Pitch = FMath::FInterpTo(CurrentRotation.Pitch, 0.0f, DeltaTime, 5.0f);
		SetActorRotation(CurrentRotation);
	}
}


