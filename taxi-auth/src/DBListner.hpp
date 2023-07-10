#pragma once

#include <userver/components/component.hpp>
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/component.hpp>


#include <string>
#include <string_view>
#include <vector>

namespace userver::formats::postgre::response{
    constexpr auto kCreateTable = R"~(
            CREATE TABLE IF NOT EXISTS user_db (
                id serial PRIMARY KEY,
                login VARCHAR,
                password VARCHAR,
                driver boolean,
                admin boolean
            )
        )~";

    const userver::storages::postgres::Query checkQuery{"SELECT id, password, driver, admin FROM user_db WHERE login=$1",
                                                        userver::storages::postgres::Query::Name{"sample_check"}};

    const userver::storages::postgres::Query registerQuery{"INSERT INTO user_db VALUES (DEFAULT, $1, $2, $3, $4)",
                                                        userver::storages::postgres::Query::Name{"sample_reg"}};

    const userver::storages::postgres::Query changeQuery{"UPDATE user_db SET password=$1 WHERE login=$2",
                                                        userver::storages::postgres::Query::Name{"sample_check"}};
}

namespace DataBaseListner{

    class DBListner final : public userver::server::handlers::HttpHandlerBase {
    public:
        static constexpr std::string_view kName = "dblistner";

        DBListner(const userver::components::ComponentConfig&,
                  const userver::components::ComponentContext&);

        std::string HandleRequestThrow(const userver::server::http::HttpRequest&,
                                       userver::server::request::RequestContext&) const override;

        std::string checkUser(const userver::server::http::HttpRequest&) const;
        std::string registerUser(const userver::server::http::HttpRequest&) const;
        std::string changePassword(const userver::server::http::HttpRequest&) const;

    private:
        userver::storages::postgres::ClusterPtr pg_cluster_;
    };

}