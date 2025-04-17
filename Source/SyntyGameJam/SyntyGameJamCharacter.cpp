// Copyright Epic Games, Inc. All Rights Reserved.

#include "SyntyGameJamCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
//#include "Actors/BaseProjectile.h"
//#include "Actors/SJAttributeComponent.h"

ASyntyGameJamCharacter::ASyntyGameJamCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	//Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	//Weapon->SetupAttachment(GetMesh(), FName("RightHandSocket"));
	////TODO: Check, not sure about this...
	//Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	//AttributeComponent = CreateDefaultSubobject<USJAttributeComponent>("AttributeComp");
}

void ASyntyGameJamCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

//void ASyntyGameJamCharacter::FireProjectile()
//{
//	if (!bCanFire || Bullets <= 0)
//	{
//		return;
//	}
//
//	bCanFire = false;
//	GetWorld()->GetTimerManager().SetTimer(FireCooldownHandle, this, &ThisClass::ResetFire, FireCooldownSeconds, false);
//
//	if (!ProjectileClass)
//	{
//		return;
//	}
//
//	FVector WorldLocation, WorldDirection;
//	APlayerController* PC = Cast<APlayerController>(GetController());
//
//	if (PC && PC->DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
//	{
//		// TODO: 1. refactor que no se vuelva a llamar de nuevo el controller desde aca.
//		// TODO: 2. que las balas "salgan Chatas" no elevadas si el mouse esta elevado ?
//		FHitResult HitResult;
//		FVector TraceStart = WorldLocation;
//		FVector TraceEnd = TraceStart + WorldDirection * 10000.f; // long range trace
//
//		FCollisionQueryParams Params;
//		Params.AddIgnoredActor(this); // Ignore self
//
//		if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, Params))
//		{
//			FVector TargetLocation = HitResult.ImpactPoint;
//
//
//			FTransform MuzzleTransform = GetMesh()->GetSocketTransform(MuzzleSocket);
//			FVector MuzzleLocation = MuzzleTransform.GetLocation();
//
//			FVector ShootDirection = (TargetLocation - MuzzleLocation).GetSafeNormal();
//			FRotator MuzzleRotation = ShootDirection.Rotation();
//
//			FActorSpawnParameters SpawnParams;
//			SpawnParams.Owner = this;
//			SpawnParams.Instigator = GetInstigator();
//
//			GetWorld()->SpawnActor<ABaseProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
//			Bullets--;
//
//			if (OnFProjectileFired.IsBound())
//			{
//				OnFProjectileFired.Broadcast();
//			}
//		}
//	}
//
//	//FTransform MuzzleTransform = GetMesh()->GetSocketTransform(MuzzleSocket);
//	//FVector SpawnLocation = MuzzleTransform.GetLocation();
//	//FRotator SpawnRotation = MuzzleTransform.GetRotation().Rotator();
//
//	//FActorSpawnParameters SpawnParams;
//	//SpawnParams.Owner = this;
//
//	//GetWorld()->SpawnActor<ABaseProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
//}

//void ASyntyGameJamCharacter::GainBullets(int32 GainedBullets)
//{
//	Bullets += GainedBullets;
//}
//
//void ASyntyGameJamCharacter::GainGoldCoins(int32 GainedCoins)
//{
//	GoldCoins += GainedCoins;
//	if (OnCoinsUpdated.IsBound())
//	{
//		OnCoinsUpdated.Broadcast(GoldCoins);
//	}
//}
//
//void ASyntyGameJamCharacter::ResetFire()
//{
//	bCanFire = true;
//}
