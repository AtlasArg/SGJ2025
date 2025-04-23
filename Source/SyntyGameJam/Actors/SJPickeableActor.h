// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SJPickeableActor.generated.h"

class UNiagaraSystem;
class USphereComponent;

UENUM(BlueprintType)
enum class EPickeableType : uint8
{
	Bullets,
	Health,
	Coins
};

UCLASS()
class SYNTYGAMEJAM_API ASJPickeableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASJPickeableActor();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category="Pick")
	EPickeableType PickeableType = EPickeableType::Bullets;

	UPROPERTY(EditAnywhere, Category = "Pick")
	int32 Amount = 1;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Collision;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> PickEffect;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> PickupSound;

private:

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResults);
};
