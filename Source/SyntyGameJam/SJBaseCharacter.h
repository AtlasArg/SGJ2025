// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SyntyGameJam/Interactions/SJPickeableReceiver.h"
#include "SJBaseCharacter.generated.h"

class USJAttributeComponent;
class USkeletalMeshComponent;
class ASJPickeableActor;
class UAnimMontage;
class ASJInteractableActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FProjectileFired);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemsUpdated, int32, itemCount);


UCLASS()
class SYNTYGAMEJAM_API ASJBaseCharacter : public ACharacter, public ISJPickeableReceiver
{
	GENERATED_BODY()

public:
	ASJBaseCharacter();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	virtual void FireProjectile(FVector Location);

	UPROPERTY(BlueprintAssignable)
	FProjectileFired OnFProjectileFired;

	UPROPERTY(BlueprintAssignable)
	FItemsUpdated OnCoinsUpdated;

	UPROPERTY(BlueprintAssignable)
	FItemsUpdated OnBulletsUpdated;

	UFUNCTION(BlueprintCallable)
	void GainBullets(int32 GainedBullets);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetBulletCount() { return Bullets; }

	UFUNCTION(BlueprintCallable)
	void GainGoldCoins(int32 GainedCoins);

	UFUNCTION(BlueprintCallable)
	void RemoveGoldCoins(int32 RemovedCoins);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetGoldCoins() { return GoldCoins; }

	virtual void ReceivePickeable_Implementation(ASJPickeableActor* Pickeable) override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateFacingTarget(const FVector& Target);

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TObjectPtr<UAnimMontage> FireMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TObjectPtr<UAnimMontage> DeathMontage;

	void FinishFiringProjectile();

	void SetFireTargetLocation(FVector FireLocation);

	bool CanMove();
	 
	void GrantReputation(int32 ReputationGranted);
	int32 GetReputation();

	void GrantHealth(float HealthGained);

	void SetCurrentInteractable(ASJInteractableActor* NewInteractable);

	virtual void InteractWithMapElement();

protected:
	
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USJAttributeComponent> AttributeComponent; 

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Drops")
	TSubclassOf<AActor> DropeableBulletsClass;

	UPROPERTY(EditAnywhere, Category = "Drops")
	TSubclassOf<AActor> DropeableCoinsClass;

	UPROPERTY(EditAnywhere, Category = "Combat")
	FName MuzzleSocket;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float FireCooldownSeconds = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	int32 Bullets = 5;

	UPROPERTY(EditAnywhere, Category = "Gold")
	int32 GoldCoins = 5;

	void ResetFire();

	bool bCanFire = true;
	bool bDead = false;
	FTimerHandle FireCooldownHandle;

	UFUNCTION()
	virtual void CharacterDied();

	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	FVector FireTargetLocation;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ASJInteractableActor> CurrentInteractable;
};
