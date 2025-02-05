#include "SpartaSpiderController.h"
#include "EnhancedInputSubsystems.h"

ASpartaSpiderController::ASpartaSpiderController()
	: InputMappingContext(nullptr),
	MoveAction(nullptr),
	AscendAction(nullptr),
	DescendAction(nullptr),
	LookAction(nullptr),
	RollAction(nullptr),
	JumpAction(nullptr),
	SprintAction(nullptr)
{
}

void ASpartaSpiderController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}
}
