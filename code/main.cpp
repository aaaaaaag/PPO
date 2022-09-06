#include "CdkCoordinator.h"
#include "AuthUserSingleton.h"
#include "windows/cdk/CdkWindowsFactory.h"
#include "facade/MainFacade.h"
#include "facade/UserFacade.h"
#include "facade/TournamentFacade.h"
#include "facade/MatchFacade.h"
#include "transactions/UserTransactionFactory.h"
#include "transactions/TournamentTransactionFactory.h"
#include "transactions/MatchTransactionFactory.h"
#include "repositories/RepositoryFactoryCreator.h"
#include "Logger.h"
#include "utility/JsonConfiguration.h"

int main() {

    polytour::db::utility::JsonConfiguration::parse("/home/denis/ppo/PPO/code/db/db_conf.json");

    auto user= polytour::bl::AuthUserSingleton::getInstance();
    user->password = "mypass";
    user->nickname = "myuser";
    polytour::transport::Logger::setLogLevel(polytour::transport::Logger::LogLevel::Info);
    polytour::transport::Logger::info("==================== run application =======================");

    auto repoFactoryCreator = std::make_shared<polytour::db::repository::RepositoryFactoryCreator>();

    auto userTransactionFactory = std::make_shared<polytour::bl::transaction::UserTransactionFactory>(repoFactoryCreator);
    auto tournamentTransactionFactory = std::make_shared<polytour::bl::transaction::TournamentTransactionFactory>(repoFactoryCreator);
    auto matchTransactionFactory = std::make_shared<polytour::bl::transaction::MatchTransactionFactory>(repoFactoryCreator);

    auto userFacade = std::make_shared<polytour::bl::facade::UserFacade>(userTransactionFactory);
    auto tournamentFacade = std::make_shared<polytour::bl::facade::TournamentFacade>(tournamentTransactionFactory);
    auto matchFacade = std::make_shared<polytour::bl::facade::MatchFacade>(matchTransactionFactory);

    auto mainFacade = std::make_shared<polytour::bl::facade::MainFacade>(
            userFacade, tournamentFacade, matchFacade);
    auto coordinator = std::make_shared<polytour::ui::CdkCoordinator>(mainFacade);
    auto windowsFactory = std::make_shared<polytour::ui::CdkWindowsFactory>(coordinator);
    coordinator->setFactory(windowsFactory);

    polytour::transport::Logger::info("==================== close application =======================");

    return 0;
}