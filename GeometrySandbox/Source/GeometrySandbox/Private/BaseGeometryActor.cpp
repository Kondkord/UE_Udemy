// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGeometryActor.h"

#include "Engine/Engine.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "TimerManager.h"

//Настройка макросом собственного лога
//уровни можно посмотреть в определении макроса - далее в определении ELogVerbosity.h
DEFINE_LOG_CATEGORY_STATIC(LogBaseGeometry, All, All);


// Sets default values
ABaseGeometryActor::ABaseGeometryActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	SetRootComponent(BaseMesh);

}

// Called when the game starts or when spawned
void ABaseGeometryActor::BeginPlay()
{
	Super::BeginPlay();

	InitialLocation = GetActorLocation();

	//PrintTypes();
	// Собственный лог	
	//PrintStringTypes();
	//PrintTransform();
	SetColor(GeometryData.Color);

	GetWorldTimerManager().SetTimer(
		TimerHandle, 
		this, 
		&ABaseGeometryActor::OnTimerFired,
		GeometryData.TimerRate,
		true);

}

void ABaseGeometryActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	UE_LOG(LogBaseGeometry, Error, TEXT("ACTOR DESTOYED is %s"), *GetName());
}

// Called every frame
void ABaseGeometryActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HandleMovement();
	
}

void ABaseGeometryActor::PrintTypes()
{
	UE_LOG(LogBaseGeometry, Warning, TEXT("Actor name %s: "), *GetName());
	UE_LOG(LogBaseGeometry, Warning, TEXT("Weapons num: %+d, kills num: %i"), WeaponsNum, KillsNum);
	UE_LOG(LogBaseGeometry, Warning, TEXT("Health: %f"), Health);
	UE_LOG(LogBaseGeometry, Warning, TEXT("isDead: %d"), isDead);
	UE_LOG(LogBaseGeometry, Warning, TEXT("Has Weapon: %d"), static_cast<int>(HasWeapon));
}

void ABaseGeometryActor::PrintStringTypes()
{
	//UE_LOG(LogBaseGeometry, Display, TEXT("info"));

	FString Name = "John Connor";
	UE_LOG(LogBaseGeometry, Display, TEXT("Name: %s"), *Name);

	FString WeaponNumStr = "Weapon num = " + FString::FromInt(WeaponsNum);
	FString HealthStr = "Health = " + FString::SanitizeFloat(Health, 2);
	FString isDeadStr = "Is Dead = " + isDead ? TEXT("true") : TEXT("false");
	//FString isDeadStr = "Is Dead = " + FString(isDead ? "true" : "false");

	FString Stat = FString::Printf(TEXT(" \n-- All Stat -- \n%s \n%s \n%s"), *WeaponNumStr, *HealthStr, *isDeadStr);

	UE_LOG(LogBaseGeometry, Display, TEXT("%s"), *WeaponNumStr);
	UE_LOG(LogBaseGeometry, Display, TEXT("%s"), *HealthStr);
	UE_LOG(LogBaseGeometry, Display, TEXT("%s"), *isDeadStr);
	UE_LOG(LogBaseGeometry, Warning, TEXT("%s"), *Stat);

	//Другой способ добавления информации на дисплей
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, Name);
		GEngine->AddOnScreenDebugMessage(-1, 7.0f, FColor::Green, Stat, true, FVector2D(1.5f, 1.5f));
	}
}

void ABaseGeometryActor::PrintTransform()
{

	FTransform Transform = GetActorTransform();
	FVector Location = Transform.GetLocation();
	FRotator Rotation = Transform.Rotator();
	FVector Scale = Transform.GetScale3D();
	UE_LOG(LogBaseGeometry, Warning, TEXT("Actor name: %s"), *GetName());
	UE_LOG(LogBaseGeometry, Warning, TEXT("Transform: %s"), *Transform.ToString());
	UE_LOG(LogBaseGeometry, Warning, TEXT("Location: %s"), *Location.ToString());
	UE_LOG(LogBaseGeometry, Warning, TEXT("Rotate: %s"), *Rotation.ToString());
	UE_LOG(LogBaseGeometry, Warning, TEXT("Scale: %s"), *Scale.ToString());

	UE_LOG(LogBaseGeometry, Error, TEXT("Human transform: %s"), *Transform.ToHumanReadableString());



}

void ABaseGeometryActor::HandleMovement()
{
	switch (GeometryData.MoveType) {
	case EMovementType::Sin: {
		FVector CurrentLocation = GetActorLocation();
		if (GetWorld()) {
			float Time = GetWorld()->GetTimeSeconds();
			CurrentLocation.Z = InitialLocation.Z + GeometryData.Amplitude
				* FMath::Sin(GeometryData.Freq * Time);
			SetActorLocation(CurrentLocation);
		}
		
		break;
	}
	case EMovementType::Static: {
		break;
	}
	default:
		break;
	}
}

void ABaseGeometryActor::SetColor(const FLinearColor& Color)
{
	if (!BaseMesh) return;

	UMaterialInstanceDynamic* DinMaterial = BaseMesh->CreateAndSetMaterialInstanceDynamic(0);
	if (DinMaterial) {
		DinMaterial->SetVectorParameterValue("Color", Color);
	}
}

void ABaseGeometryActor::OnTimerFired()
{
	if (++TimerCount <= MaxTimerCount)
	{
		FLinearColor NewColor = FLinearColor::MakeRandomColor();
		UE_LOG(LogBaseGeometry, Display, TEXT("Color to set up: %s"), *NewColor.ToString());
		SetColor(NewColor);
		OnColorChanged.Broadcast(NewColor, GetName());
	}
	else {
		UE_LOG(LogBaseGeometry, Display, TEXT("TIMER STOP"));
		GetWorldTimerManager().ClearTimer(TimerHandle);
		OnTimerFinished.Broadcast(this);
	}
}

