// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SJBaseCharacter.h"
#include "SyntyGameJamCharacter.generated.h"

class USkeletalMeshComponent;
class UCameraComponent;
//class USJAttributeComponent;

//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FProjectileFired);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCoinsUpdated, int32, coins);

UCLASS(Blueprintable)
class ASyntyGameJamCharacter : public ASJBaseCharacter
{
	GENERATED_BODY()

public:
	ASyntyGameJamCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	//void FireProjectile();

	//UPROPERTY(BlueprintAssignable)
	//FProjectileFired OnFProjectileFired;

	//UPROPERTY(BlueprintAssignable)
	//FCoinsUpdated OnCoinsUpdated;
	//
	//UFUNCTION(BlueprintCallable)
	//void GainBullets(int32 GainedBullets);

	//UFUNCTION(BlueprintCallable)
	//FORCEINLINE int32 GetBulletCount() { return Bullets; }

	//UFUNCTION(BlueprintCallable)
	//void GainGoldCoins(int32 GainedCoins);

	//UFUNCTION(BlueprintCallable)
	//FORCEINLINE int32 GetGoldCoins() { return GoldCoins; }

//protected:
//	UPROPERTY(EditAnywhere, Category = "Combat")
//	TObjectPtr<USkeletalMeshComponent> Weapon;

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> TopDownCameraComponent;

	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USJAttributeComponent> AttributeComponent;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Combat")
	FName MuzzleSocket;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float FireCooldownSeconds = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	int32 Bullets = 5;

	UPROPERTY(EditAnywhere, Category = "Gold")
	int32 GoldCoins = 5;*/

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	//void ResetFire();

	//bool bCanFire = true;
	//FTimerHandle FireCooldownHandle;
};

