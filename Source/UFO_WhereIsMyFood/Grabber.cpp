// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// Creating particle component
	GrabParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("GrabParticleComponent"));
	
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	UPhysicsHandleComponent* PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	if (PhysicsHandle == nullptr)
	{
		return;
	}

	if (PhysicsHandle->GetGrabbedComponent() != nullptr)
	{
		FVector TargetLocation = GetComponentLocation() + GetForwardVector() * HoldDistance;
		PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
	}
}

void UGrabber::Grab()
{
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	if (PhysicsHandle == nullptr)
	{
		return;
	}

	FHitResult HitResult;
	bool bHasHIt = GetGrabbableInreach(HitResult);
	if (bHasHIt)
	{
		//DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 100, 10, FColor::Red, false, 5);
		UPrimitiveComponent* HitComponent = HitResult.GetComponent();
		// WakeHitResult component rigidbody for physics interaction
		HitComponent->WakeAllRigidBodies();
		// Grab HitComponent
		PhysicsHandle->GrabComponentAtLocationWithRotation(HitComponent, NAME_None, HitResult.ImpactPoint, GetComponentRotation());
	}

	// Spawn and attach the particle system to the grabber
	if (GrabParticleComponent && GrabParticleComponent->Template)
	{
		// Spawn the particle system at the grabber's location
		GrabParticleComponent->SetRelativeLocation(FVector::ZeroVector); // Offset if needed
		GrabParticleComponent->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
		GrabParticleComponent->Activate(true);
		// Set scale and rotation of particles
		FVector InitialScale = FVector(3.0f, 3.0f, 3.0f);
		GrabParticleComponent->SetRelativeScale3D(InitialScale);
		FRotator NewRotation = FRotator(0.0f, 90.0f, -90.0f);
		GrabParticleComponent->SetRelativeRotation(NewRotation);
	}
	
}

void UGrabber::Released()
{
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	if (PhysicsHandle == nullptr)
	{
		return;
	}

	if (PhysicsHandle->GetGrabbedComponent() != nullptr)
	{
		PhysicsHandle->GetGrabbedComponent()->WakeAllRigidBodies();
		PhysicsHandle->ReleaseComponent();
	}

	// Deactivate and detach the particle system
	if (GrabParticleComponent)
	{
		GrabParticleComponent->Deactivate();
		GrabParticleComponent->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
	}
}

UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const
{
	UPhysicsHandleComponent* Result =  GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (Result == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT(" Grabber requires a UPhysicsHandleComponent"));
	}
	return Result;
}

// Check if we have the grabbable in reach
bool UGrabber::GetGrabbableInreach(FHitResult& OutHitResult) const
{
	FVector	Start = GetComponentLocation();
	FVector End = Start + GetForwardVector() * MaxGrabDistance;
	//DrawDebugLine(GetWorld(), Start, End, FColor::Red);
	//DrawDebugSphere(GetWorld(), End, 100, 10, FColor::Blue, false, 5);

	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);
	return GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, ECC_GameTraceChannel1, Sphere); 
}


