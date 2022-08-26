//
// Created by denis on 7/12/22.
//

#ifndef COURSEWORKDB_IREPOSITORY_H
#define COURSEWORKDB_IREPOSITORY_H

#include <vector>

namespace polytour::db::repository {
    template<class T>
    class IRepository {
    public:
        virtual ~IRepository() = default;

        virtual void addObj(const T& obj) = 0;
        virtual void deleteObj(const T& obj) = 0;
        virtual void updateObj(const T& curObj, const T& newObj) = 0;
        virtual std::vector<T> findObj(const typename T::search_t& obj) = 0;
    };
}

#endif //COURSEWORKDB_IREPOSITORY_H
