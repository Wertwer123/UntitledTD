#include "TDPlayerController.h"
#include "TDPlayerInputComponent.h"
#include "Kismet/KismetMathLibrary.h"

DEFINE_LOG_CATEGORY(LogPlayerInput)

ATDPlayerController::ATDPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;

	PlayerInputComponent = Cast<UTDPlayerInputComponent>(CreateDefaultSubobject<UTDPlayerInputComponent>(FName("PlayerInputComponent")));
	InputComponent = PlayerInputComponent;
}

void ATDPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	PlayerInputComponent->SetUpPlayerInput(this);
	PlayerInputComponent->BindAction(RotateCameraAction, ETriggerEvent::Triggered, this, &ATDPlayerController::RotateCamera);
	PlayerInputComponent->BindAction(ZoomInCamera, ETriggerEvent::Triggered, this, &ATDPlayerController::ZoomCamera);
}

void ATDPlayerController::ComputeCameraMovement(const float DeltaSeconds)
{
	int32 XScreenSize;
	int32 YScreenSize;
	GetViewportSize(XScreenSize, YScreenSize);

	float MouseX;
	float MouseY;
	GetMousePosition(MouseX, MouseY);

	//Calculate the final position of our camera
	PositionToReach += GetCameraXYMovement(XScreenSize, YScreenSize, MouseX, MouseY);
	AdjustCameraToGroundHeight(DeltaSeconds);
	
	MoveCamera();
}

void ATDPlayerController::MoveCamera()
{
	GetPawn()->SetActorLocation(PositionToReach);
}

void ATDPlayerController::ZoomCamera(const FInputActionValue& InputActionValue)
{
	float ActionValue = InputActionValue.Get<float>();
	
	HeightOffsetCamera -= ActionValue * ScrollSpeed;
	HeightOffsetCamera = FMath::Clamp(HeightOffsetCamera, MinHeightOffsetCamera, MaxHeightOffsetCamera);
}

void ATDPlayerController::RotateCamera(const FInputActionValue& InputActionValue)
{
	float RotationAmount = InputActionValue.Get<float>();

	CurrentDegreesOnCircle += RotationAmount * RotationSpeed * GetWorld()->GetDeltaSeconds();
	
	FHitResult HitResult = GroundRayCast(PositionToReach);
	FTransform CenterOfCircle = FTransform::Identity;
	const FVector GroundHitLocation = HitResult.Location;
	const FVector PlayerLocation = GetPawn()->GetActorLocation();
	CenterOfCircle.SetLocation(GroundHitLocation);

	//Calculate the radius of the circle using pythagoras
	double A = (PlayerLocation.Z - GroundHitLocation.Z);
	double C = (GroundHitLocation - PlayerLocation).Length();
	double B =  FMath::Sqrt(FMath::Square(C) - FMath::Square(A));
	
	double RadiusOfCircle = B;

	//then get the position on that circle by our rotated degrees
	double RotationAmountInRadians = FMath::DegreesToRadians(CurrentDegreesOnCircle);
	double XCoordinateOnCircle = RadiusOfCircle * FMath::Sin(RotationAmountInRadians);
	double YCoordinateOnCircle = RadiusOfCircle * FMath::Cos(RotationAmountInRadians);

	//then get the position of the circle relative to the center of the Circle
	FVector NewPosOnCircle(XCoordinateOnCircle, YCoordinateOnCircle, 0);
	FVector NewPosOnCircleWorldSpaceRelativeToCircleCenter = CenterOfCircle.TransformPosition(NewPosOnCircle);
	NewPosOnCircleWorldSpaceRelativeToCircleCenter.Z = PlayerLocation.Z;
	
	FVector DirectionToNewPos = NewPosOnCircleWorldSpaceRelativeToCircleCenter - PlayerLocation;
	
	//then clcualate the direction towards it and add it to the position to reach
	PositionToReach += DirectionToNewPos;

	//also set the rotation so that we area alway looking towards the center of rotation
	FVector DirectionToCenter = CenterOfCircle.GetLocation() - PositionToReach;
	FRotator LookAtRotation = DirectionToCenter.Rotation();
	GetPawn()->SetActorRotation(LookAtRotation);
}

void ATDPlayerController::AdjustCameraToGroundHeight(const float DeltaTime)
{
	FHitResult GroundHitResult = GroundRayCast(PositionToReach);
	
	if(GroundHitResult.bBlockingHit)
	{
		const float ToHeightOffSet = UKismetMathLibrary::FInterpTo(GetPawn()->GetActorLocation().Z, GroundHitResult.Location.Z + HeightOffsetCamera, DeltaTime, InterpolationSpeed);
		PositionToReach.Z = ToHeightOffSet;
	}
}

void ATDPlayerController::LockMouseCursorToViewPort()
{
	FInputModeGameAndUI NewInputMode;
	NewInputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);

	SetInputMode(NewInputMode);
}

FHitResult ATDPlayerController::GroundRayCast(const FVector& From)
{
	FHitResult OutHitResult;
	
	if(UWorld* World = GetWorld())
	{
		
		const FVector RayCastEnd = From + GetPawn()->GetActorForwardVector() *  LineTraceLength;

		DrawDebugLine(World, GetPawn()->GetActorLocation(), RayCastEnd, FColor::Red);
		World->LineTraceSingleByChannel(OutHitResult, GetPawn()->GetActorLocation(), RayCastEnd, GroundCollisionChannel);
	}

	return OutHitResult;
}

FVector ATDPlayerController::GetCameraXYMovement(const int32 XViewPortSize, const int32 YViewPortSize,
                                                 const float MouseX, const float MouseY)
{
	FVector MovementVector = FVector::Zero();
	
	float MouseXPercentage = MouseX / XViewPortSize;
	float MouseYPercentage = MouseY / YViewPortSize;

	//Clamp the percentages because although we have the cursor locked we sometimes get some very fucked up values
	//also consider maybe just returning zero if we get such a value because it doesnt matter if we dont move for one frame ocassionaly
	MouseXPercentage = FMath::Clamp(MouseXPercentage, 0.f, 1.f);
	MouseYPercentage = FMath::Clamp(MouseYPercentage, 0.f, 1.f);
	
	//Calculate a vector from the center of the viewport to the mouse position
	const float ViewportCenterX = static_cast<float>(XViewPortSize / 2);
	const float ViewportCenterY = static_cast<float>( YViewPortSize / 2);

	const FVector ViewportCenter(ViewportCenterY, ViewportCenterX, 0);
	const FVector MousePos(MouseY, MouseX, 0);
	
	FVector DirectionToMouse = (MousePos - ViewportCenter);
	DirectionToMouse.X *= -1;

	if(MouseXPercentage > MaxStartScrollPercentage || MouseYPercentage > MaxStartScrollPercentage
	|| MouseXPercentage < MinStartScrollPercentage || MouseYPercentage < MinStartScrollPercentage)
	{
		const FVector MovementVectorInWorldSpaceRelativeToPlayer = GetPawn()->GetActorTransform().TransformPosition(DirectionToMouse);
		MovementVector = (MovementVectorInWorldSpaceRelativeToPlayer - GetPawn()->GetActorLocation()).GetSafeNormal() * Speed;
	}
	
	return MovementVector;
}

void ATDPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	LockMouseCursorToViewPort();
	HeightOffsetCamera = MaxHeightOffsetCamera;
	PositionToReach = GetPawn()->GetActorLocation();
}

void ATDPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	ComputeCameraMovement(DeltaSeconds);
	GetPawn()->SetActorLocation(PositionToReach);
}


