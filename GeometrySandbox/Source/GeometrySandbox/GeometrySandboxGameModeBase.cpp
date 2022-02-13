// Copyright Epic Games, Inc. All Rights Reserved.


#include "GeometrySandboxGameModeBase.h"
#include "SandBoxPawn.h"
#include "SandBoxPlayerController.h"

AGeometrySandboxGameModeBase::AGeometrySandboxGameModeBase() {
	//�������������� default ����� pawn 
	DefaultPawnClass = ASandBoxPawn::StaticClass();
	PlayerControllerClass = ASandBoxPlayerController::StaticClass();
}