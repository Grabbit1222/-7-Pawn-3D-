#include "SpartaGameMode.h"
#include "SpartaCharacter.h"
#include "SpartaPlayerController.h"
#include "SpartaPawn.h"
#include "SpartaSpiderController.h"

ASpartaGameMode::ASpartaGameMode()
{
	//DefaultPawnClass = ASpartaCharacter::StaticClass();
	//PlayerControllerClass = ASpartaPlayerController::StaticClass();

	//�����̴� ���//
	DefaultPawnClass = ASpartaPawn::StaticClass();
	PlayerControllerClass = ASpartaSpiderController::StaticClass();

}