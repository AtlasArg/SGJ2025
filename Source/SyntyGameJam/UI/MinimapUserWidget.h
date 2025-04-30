// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SJIconInMapWidget.h"
#include "MinimapUserWidget.generated.h"

class UOverlay;
class UImage;
class ASyntyGameJamPlayerController;
class ASJBaseCharacter;

UCLASS()
class SYNTYGAMEJAM_API UMinimapUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable)
	USJIconInMapWidget* AddIconToMap(EMapIconType MapIcon, AActor* RelatedActor);

	UFUNCTION(BlueprintCallable)
	void AddIconsToMap();

	UFUNCTION(BlueprintCallable)
	void AddNewEnemyIconToMap(ASJBaseEnemy* NewEnemy);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void UpdateIconRotation(USJIconInMapWidget* IconToRotate);

	void UpdateEnemyIcons();
	void UpdatePointsOfInterestIcons();

	FVector2D GetActorScreenOffset(FVector ActorLocation, FVector CameraLocation, FRotator CameraRotation);

	virtual void UpdateIcons(TArray<USJIconInMapWidget*>& IconsInMap, bool bHideIfHavingTag, bool bUpdateColor);

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UOverlay> MapOverlay = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UImage> MapImage = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	TSubclassOf<USJIconInMapWidget> DefaultIconWidgetClass;

	TObjectPtr<USJIconInMapWidget> PlayerIcon;
	TArray<TObjectPtr<USJIconInMapWidget>> EnemyIcons;
	TArray<TObjectPtr<USJIconInMapWidget>> PointsOfInterestIcons;

	float HalfUISizeInX = 0.f;
	float HalfUISizeInY = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	float HalfCameraWidth = 4000.f;

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	float MapSizeBoxContainerSize = 256.f;
};
