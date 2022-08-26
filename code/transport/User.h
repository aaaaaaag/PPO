//
// Created by denis on 7/22/22.
//

#ifndef COURSEWORKDB_USER_H
#define COURSEWORKDB_USER_H

#include "IIdentifiable.h"
#include "NotMandatoryField.h"
#include <string>
#include <optional>

namespace polytour::transport {
    class User: public IIdentifiable{
    public:
        int id;
        std::string name;
        utility::NotMandatoryField<std::string> surname;
        std::string nickname;
        std::string email;
        utility::NotMandatoryField<int> age;
        std::string password;

        int getOID() override {return id;}

        struct SearchUser{
            std::optional<decltype(id)> id_;
            std::optional<decltype(name)> name_;
            std::optional<decltype(surname)> surname_;
            std::optional<decltype(password)> password_;
            std::optional<decltype(nickname)> nickname_;
            std::optional<decltype(age)> age_;
            std::optional<decltype(email)> email_;
        };

        using search_t = SearchUser;
    };
}

#endif //COURSEWORKDB_USER_H
