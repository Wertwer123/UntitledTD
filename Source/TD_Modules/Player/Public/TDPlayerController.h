// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InputMappingContext.h"
#include "TDPlayerController.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPlayerInput, Log, All);

class UTDPlayerInputComponent;

//Camera in the context of this player controller directly translates to our player
UCLASS()
class PLAYER_API ATDPlayerController : public APlayerController
{
	GENERATED_BODY()


public:
	ATDPlayerController();

	TObjectPtr<UTDPlayerInputComponent> GetPlayerInputComponent() {return PlayerInputComponent;}
	
private:

	UPROPERTY(EditDefaultsOnly, meta=(ClampMin = 0, ClampMax = 360), Category= "Camera")
	float StartDegrees = 0.0f;
	
	UPROPERTY(EditAnywhere, meta=(ClampMin = 0, ClampMax = 1), Category= "Movement")
	float MinStartScrollPercentage = 0.1f;

	UPROPERTY(EditAnywhere, meta=(ClampMin = 0, ClampMax = 1), Category= "Movement")
	float MaxStartScrollPercentage = 0.9f;
	
	UPROPERTY(EditAnywhere, meta=( ClampMin = 0), Category= "Movement")
	double Speed = 10;

	UPROPERTY(EditAnywhere, meta=( ClampMin = 0), Category= "Movement")
	double RotationSpeed = 20;

	UPROPERTY(EditAnywhere, meta=( ClampMin = 0), Category= "Movement")
	double ScrollSpeed = 10;
	
	UPROPERTY(EditAnywhere, meta=( ClampMin = 0), Category= "Camera")
	double HeightOffsetCamera = 10;

	UPROPERTY(EditAnywhere, meta=( ClampMin = 0), Category= "Camera")
	double MinHeightOffsetCamera = 200;

	UPROPERTY(EditAnywhere, meta=( ClampMin = 0), Category= "Camera")
	double MaxHeightOffsetCamera = 10;
	
	UPROPERTY(EditAnywhere, meta=( ClampMin = 0), Category= "Camera|Traces")
	double LineTraceLength = 20000;
	
	UPROPERTY(EditAnywhere, meta=( ClampMin = 0), Category= "Camera")
	double InterpolationSpeed = 5;

	UPROPERTY()
	FVector PositionToReach = FVector::Zero();
	
	UPROPERTY(EditAnywhere, Category= "Camera")
	TEnumAsByte<ECollisionChannel> GroundCollisionChannel;
	
	UPROPERTY(EditAnywhere, Category= "Input")
	TObjectPtr<UTDPlayerInputComponent> PlayerInputComponent;

	UPROPERTY(EditAnywhere, Category= "Input")
	TObjectPtr<UInputAction> RotateCameraAction;
	
	UPROPERTY(EditAnywhere, Category= "Input")
	TObjectPtr<UInputAction> ZoomInCamera;
	
	float CurrentDegreesOnCircle = 0;
	float CircleRadius = 0;
	
	void MoveCamera(const float DeltaSeconds);
	void ZoomCamera(const FInputActionValue& InputActionValue);
	void RotateCamera(const FInputActionValue& InputActionValue);
	void SetCameraPosition(const float Degrees);
	void ComputeCameraMovement(const float DeltaSeconds);
	void AdjustCameraToGroundHeight(const float DeltaTime);
	void LockMouseCursorToViewPort();

	///Shoots a raycast in the forward direction of the player
	FHitResult GroundRayCast(const FVector& From, const FVector& Direction);
	FVector GetCameraXYMovement(const int32 XViewPortSize, const int32 YViewPortSize, const float MouseX, const float MouseY);
	
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaSeconds) override;
};
