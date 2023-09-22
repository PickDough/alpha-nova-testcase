// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InputActionValue.h"
#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Pawn.h"
#include "Sphere.generated.h"


UCLASS()
class ALPHANOVEL_TESTCASE_API APSphere : public APawn
{
	GENERATED_BODY()

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Movement */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Movement, meta = (AllowPrivateAccess = "true"))
	double RollingSpeed = 2.5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Movement, meta = (AllowPrivateAccess = "true"))
	double ArrowMinLength = 60.;
	

protected:
	UStaticMeshComponent* BodyComponent;

	UArrowComponent* Arrow;
	
public:
	// Sets default values for this pawn's properties
	APSphere();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void Look(const FInputActionValue& Value);
	void Move(const FInputActionValue& Value);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
