//
// Created by denis on 25.08.2022.
//

#ifndef COURSEWORKDB_MOCKREPOSITORY_H
#define COURSEWORKDB_MOCKREPOSITORY_H

#include "repositories/IRepository.h"
#include "gmock/gmock.h"

namespace polytour::db::tests::mocks {
    template <class T>
    class MockRepository: public repository::IRepository<T> {
    public:
        MOCK_METHOD(void, addObj, (const T& obj), (override));
        MOCK_METHOD(void, deleteObj, (const T& obj), (override));
        MOCK_METHOD(void, updateObj, (const T& curObj, const T& newObj), (override));
        MOCK_METHOD(std::vector<T>, findObj, (const typename T::search_t& obj), (override));
    };
}

#endif //COURSEWORKDB_MOCKREPOSITORY_H
