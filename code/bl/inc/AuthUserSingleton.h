//
// Created by denis on 03.08.2022.
//

#ifndef COURSEWORKDB_AUTHUSERSINGLETON_H
#define COURSEWORKDB_AUTHUSERSINGLETON_H

#include "User.h"
#include <memory>
#include <string>

namespace polytour::bl {
    class AuthUserSingleton {
    private:

        AuthUserSingleton() = default;

        AuthUserSingleton(const AuthUserSingleton&) = default;
        AuthUserSingleton& operator = (AuthUserSingleton &) = default;

        //static std::unique_ptr<transport::User> _pInstance;

    public:

        static void authorize(const transport::User& user) {
            getInstance()->id = user.id;
            getInstance()->name = user.name;
            getInstance()->nickname = user.nickname;
            getInstance()->password = user.password;
            getInstance()->email = user.email;
            getInstance()->age = user.age;
            getInstance()->surname = user.surname;
        };

        static transport::User* getInstance() {
            static std::unique_ptr<transport::User> _pInstance = std::make_unique<transport::User>();

            return _pInstance.get();
        };
    };
}

#endif //COURSEWORKDB_AUTHUSERSINGLETON_H
