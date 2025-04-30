// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SJIconInMapWidget.generated.h"

class UImage;
class UTexture2D;

UENUM(BlueprintType)
enum class EMapIconType : uint8
{
	ENEMY,
	PLAYER,
	POINT_OF_INTEREST
};

UCLASS()
class SYNTYGAMEJAM_API USJIconInMapWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetupWidget(EMapIconType IconType, AActor* RelatedActor);

	UPROPERTY(BlueprintReadwrite, EditAnywhere)
	EMapIconType MapIconType = EMapIconType::PLAYER;

	UPROPERTY(BlueprintReadwrite, EditAnywhere)
	TObjectPtr<AActor> RelatedWidgetActor;

	int32 GetTeamOwnerID();

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UImage> IconImage;

	/* Player Icon used to show player in the map and minimap */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	TObjectPtr<UTexture2D> PlayerIcon;

	/* Icon used to show enemies in the map and minimap */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	TObjectPtr<UTexture2D> EnemyIcon;

	/* Icon used to show Points of interest in the map and minimap */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	TObjectPtr<UTexture2D> PointsOfInterestIcon;
};
