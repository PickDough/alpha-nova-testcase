// Fill out your copyright notice in the Description page of Project Settings.


#include "Sphere.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
APSphere::APSphere()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APSphere::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
		
		BodyComponent = CastChecked<UStaticMeshComponent>(GetRootComponent());
		Arrow = FindComponentByClass<UArrowComponent>();
		Arrow->SetWorldRotation(PlayerController->PlayerCameraManager->GetCameraRotation());
	}
}

// Called every frame
void APSphere::Tick(float DeltaTime)
{
	auto velocity = BodyComponent->GetPhysicsLinearVelocity();
	UE_LOG(LogTemp, Warning, TEXT("Velocity: %f"), velocity.Length())
	Arrow->SetWorldRotation((velocity * FVector(1.f ,1.f, 0.)).Rotation());
	Arrow->ArrowLength = ArrowMinLength + velocity.Length() * 0.05f;
	Arrow->MarkRenderStateDirty();
}

// Called to bind functionality to input
void APSphere::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APSphere::Look);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APSphere::Move);
	}
}

void APSphere::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerPitchInput(LookAxisVector.Y);
		AddControllerYawInput(LookAxisVector.X);
	}
}

void APSphere::Move(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		auto cameraForward = PlayerController->PlayerCameraManager->GetCameraRotation();

		BodyComponent->AddForce(
			RollingSpeed * BodyComponent->GetMass() * cameraForward.RotateVector(
				FVector(LookAxisVector.Y, LookAxisVector.X, 0.f)) * FVector(1.f, 1.f, 0.));
	}
}
