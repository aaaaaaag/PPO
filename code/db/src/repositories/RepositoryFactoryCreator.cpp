//
// Created by denis on 22.08.2022.
//

#include "public/repositories/RepositoryFactoryCreator.h"
#include "public/repositories/RepositoryFactory.h"
#include "public/repositories/roles/RolesConverter.h"

std::shared_ptr<polytour::db::repository::IRepositoryFactory>
polytour::db::repository::RepositoryFactoryCreator::create(polytour::transport::Roles role) {
    return std::make_shared<RepositoryFactory>(roles::RolesConverter::convert(role));
}
