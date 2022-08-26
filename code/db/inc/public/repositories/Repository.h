//
// Created by denis on 7/12/22.
//

#ifndef COURSEWORKDB_REPOSITORY_H
#define COURSEWORKDB_REPOSITORY_H

#include "IRepository.h"
#include "IRepositoryInitializer.h"
#include "IConnection.h"
#include "utility/FieldSet.h"
#include "Identity.h"
#include <memory>

namespace polytour::db::repository {
    template <class TValue>
    class Repository: public IRepository<TValue>, public IRepositoryInitializer {
    public:

        using value_t = TValue;

        explicit Repository(std::shared_ptr<IConnection> conn):
        _pConn(std::move(conn))
        {}

        void addObj(const TValue &obj) override {
            auto fieldSet = toFieldSet(obj);
            _pConn->addObj(fieldSet, getIdentity());
        }

        void deleteObj(const TValue &obj) override {
            auto fieldSet = toFieldSet(obj);
            _pConn->deleteObj(fieldSet, getIdentity());
        }

        void updateObj(const TValue& curObj, const TValue& newObj) override {
            auto curFieldSet = toFieldSet(curObj);
            auto newFieldSet = toFieldSet(newObj);
            _pConn->updateObj(curFieldSet, newFieldSet, getIdentity());
        }

        std::vector<TValue> findObj(const typename TValue::search_t &obj) override {
            auto fieldSet = toFieldSet(obj);
            auto foundFieldSets = _pConn->searchObj(fieldSet, getIdentity());
            std::vector<TValue> result;
            for (const auto& foundFieldSet: foundFieldSets)
                result.template emplace_back(fromFieldSet(foundFieldSet));
            return result;
        }

        void init() override {
            _pConn->init(getIdentity());
        }

    protected:

        virtual utility::FieldSet toFieldSet(const TValue& obj) = 0;
        virtual utility::FieldSet toFieldSet(const typename TValue::search_t& obj) = 0;
        virtual TValue fromFieldSet(const utility::FieldSet& field) = 0;
        virtual Identity getIdentity() = 0;

    private:
        std::shared_ptr<IConnection> _pConn;
    };
}


#endif //COURSEWORKDB_REPOSITORY_H
