// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "Engine/PointLight.h"
#include "Subsystems/WorldSubsystem.h"
#include "ColoredPawnWorldSubsystem.generated.h"


UENUM(BlueprintType)
enum class EColor : uint8
{
	VE_Red UMETA(DisplayName="Red"),
	VE_Green UMETA(DisplayName="Green"),
	VE_Blue UMETA(DisplayName="Blue"),
	VE_White UMETA(DisplayName = "White"),
};

inline FString GetColorEnumAsString(EColor EnumValue)
{
	switch (EnumValue)
	{
	case EColor::VE_Red:
		return "Red";
	case EColor::VE_Green:
		return "Green";
	case EColor::VE_Blue:
		return "Blue";
	case EColor::VE_White:
		return "White";
	default:
		return "Invalid";
	}
}

#define LOG_SCREEN(x) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT(x));}
/**
 * 
 */
UCLASS()
class ALPHANOVEL_TESTCASE_API UColoredPawnWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

private:
	TMap<EColor, TArray<TTuple<UStaticMeshComponent*, FString>>*> PawnColors;
	TMap<EColor, int> ColorCounts;

	TArray<APointLight*> Lights;

	const float LightIntensity = 20.f;
	const float AttentiveRadius = 1750.f;

	APointLight* GetPointLightByColor(EColor color);

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void PostInitialize() override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

	UFUNCTION(BlueprintCallable, Category="Recolorable")
	bool HasColoredAllPawns();
	
	UFUNCTION(BlueprintCallable, Category="Recolorable")
	void RegisterPawn(UObject* object, EColor color);


	UFUNCTION(BlueprintCallable, Category="Recolorable")
	void OnRecolor(EColor oldColor, EColor newColor);

	UFUNCTION(BlueprintCallable, Category="ColorInformation")
	int GetColorCount(EColor color);
};
