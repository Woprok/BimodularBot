// ©FIT Team

#include "Menu/MenuGameMode.h"
#include "Menu/MenuPlayerController.h"
#include "Menu/MenuHUD.h"

AMenuGameMode::AMenuGameMode()
{
	HUDClass = AMenuHUD::StaticClass();
	PlayerControllerClass = AMenuPlayerController::StaticClass();
}