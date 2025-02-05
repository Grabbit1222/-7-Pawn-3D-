#include "SpartaGameMode.h"
#include "SpartaCharacter.h"
#include "SpartaPlayerController.h"
#include "SpartaPawn.h"
#include "SpartaSpiderController.h"

ASpartaGameMode::ASpartaGameMode()
{
	//DefaultPawnClass = ASpartaCharacter::StaticClass();
	//PlayerControllerClass = ASpartaPlayerController::StaticClass();

	//스파이더 모드//
	DefaultPawnClass = ASpartaPawn::StaticClass();
	PlayerControllerClass = ASpartaSpiderController::StaticClass();

}