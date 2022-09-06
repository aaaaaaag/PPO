#include "CdkCoordinator.h"
#include "AuthUserSingleton.h"
#include "windows/cdk/CdkWindowsFactory.h"

int main() {
    auto user= polytour::bl::AuthUserSingleton::getInstance();
    user->password = "mypass";
    user->nickname = "myuser";

    auto coordinator = std::make_shared<polytour::ui::CdkCoordinator>();
    auto windowsFactory = std::make_shared<polytour::ui::CdkWindowsFactory>(coordinator);
    coordinator->setFactory(windowsFactory);

    return 0;
}