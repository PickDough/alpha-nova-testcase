// Fill out your copyright notice in the Description page of Project Settings.


#include "ColoredPawnWorldSubsystem.h"


#include "Components/PointLightComponent.h"
#include "Engine/PointLight.h"
#include "Kismet/GameplayStatics.h"

void UColoredPawnWorldSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UColoredPawnWorldSubsystem::PostInitialize()
{
	Super::PostInitialize();
	
	PawnColors.Add(EColor::VE_Red, new TArray<TTuple<UStaticMeshComponent*, FString>>());
	ColorCounts.Add(EColor::VE_Red, 0);
	PawnColors.Add(EColor::VE_Green, new TArray<TTuple<UStaticMeshComponent*, FString>>());
	ColorCounts.Add(EColor::VE_Green, 0);
	PawnColors.Add(EColor::VE_Blue, new TArray<TTuple<UStaticMeshComponent*, FString>>());
	ColorCounts.Add(EColor::VE_Blue, 0);
}


bool UColoredPawnWorldSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	bool bShouldCreateSubsystem = Super::ShouldCreateSubsystem(Outer);

	if (Outer)
	{
		if (UWorld* World = Outer->GetWorld())
		{
			bShouldCreateSubsystem = DoesSupportWorldType(World->WorldType) && bShouldCreateSubsystem;
		}
	}

	return bShouldCreateSubsystem;
}

void UColoredPawnWorldSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	LOG_SCREEN("WorldSubsystem Loaded")
	if (const UWorld* World = InWorld.GetWorld())
	{
		TArray<AActor*> foundActors;
		UGameplayStatics::GetAllActorsOfClass(World, APointLight::StaticClass(), foundActors);

		for (auto a : foundActors)
		{
			if (a->GetActorNameOrLabel() != GetColorEnumAsString(EColor::VE_White))
			{
				Lights.Add(CastChecked<APointLight>(a));
			}
		}
	}
}

bool UColoredPawnWorldSubsystem::HasColoredAllPawns()
{
	return ColorCounts[EColor::VE_Red] + ColorCounts[EColor::VE_Green] + ColorCounts[EColor::VE_Blue] == 0;
}

void UColoredPawnWorldSubsystem::RegisterPawn(UObject* object, EColor color)
{
	if (auto arr = *PawnColors.Find(color))
	{
		auto mesh = CastChecked<UStaticMeshComponent>(object);
		arr->Add(TTuple<UStaticMeshComponent*, FString>(mesh, mesh->GetMaterial(0)->GetFullName()));
		ColorCounts[color]++;
	}
}

APointLight* UColoredPawnWorldSubsystem::GetPointLightByColor(EColor color)
{
	for (auto p : Lights)
	{
		if (p->GetActorNameOrLabel() == GetColorEnumAsString(color))
		{
			return p;
		}
	}

	return nullptr;
}

void UColoredPawnWorldSubsystem::OnRecolor(EColor oldColor, EColor newColor)
{
	if (auto pointLight = GetPointLightByColor(oldColor))
	{
		ColorCounts[oldColor]--;
		if (auto pawns = *PawnColors.Find(oldColor))
		{
			auto recolored = 0.f;
			for (int i = 0; i < pawns->Num(); i++)
			{
				TTuple<UStaticMeshComponent*, FString> p = pawns->GetData()[i];
				if (p.Key->GetMaterial(0)->GetFullName() != p.Value)
					recolored++;
			}
			
			pointLight->PointLightComponent->Intensity = LightIntensity * (1.f - recolored / pawns->Num());
			pointLight->PointLightComponent->AttenuationRadius = AttentiveRadius * (1.f - recolored / pawns->Num());
			pointLight->MarkComponentsRenderStateDirty();
		}
	}
	
	if (auto pointLight = GetPointLightByColor(newColor))
	{
		ColorCounts[newColor]++;
		if (auto pawns = *PawnColors.Find(newColor))
		{
			auto recolored = 0;
			for (int i = 0; i < pawns->Num(); i++)
			{
				TTuple<UStaticMeshComponent*, FString> p = pawns->GetData()[i];
				if (p.Key->GetMaterial(0)->GetFullName() != p.Value)
					recolored++;
			}
	
			pointLight->PointLightComponent->Intensity = LightIntensity * (recolored / pawns->Num());
		}
	}
}

int UColoredPawnWorldSubsystem::GetColorCount(EColor color)
{
	return ColorCounts[color];
}
