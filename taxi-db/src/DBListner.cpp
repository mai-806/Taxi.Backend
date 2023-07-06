#include "DBListner.hpp"

DBListner::DBListner(const userver::components::ComponentConfig& config,
                     const userver::components::ComponentContext& context):
                     HttpHandlerBase(config, context),
                     pg_cluster_(context.FindComponent<userver::components::Postgres>("users-db").GetCluster()){

    constexpr auto kCreateTable = R"~(
            CREATE TABLE IF NOT EXISTS user_db (
                id serial PRIMARY KEY,
                login VARCHAR,
                password VARCHAR,
                driver boolean
            )
        )~";
 
    using userver::storages::postgres::ClusterHostType;
    pg_cluster_->Execute(ClusterHostType::kMaster, kCreateTable);

}


std::string DBListner::HandleRequestThrow(const userver::server::http::HttpRequest& request,
                              userver::server::request::RequestContext& context) const {

    auto method = request.GetMethod();

    switch(method){
        case(userver::server::http::HttpMethod::kGet):
            return checkUser(request);

        case(userver::server::http::HttpMethod::kPost):
            return registerUser(request);

        case(userver::server::http::HttpMethod::kDelete):
            return changePassword(request);

        default:
            return "method not found";
    }

}

const userver::storages::postgres::Query checkQuery{"SELECT password FROM user_db WHERE login=$1",
                                                        userver::storages::postgres::Query::Name{"sample_check"}};

std::string DBListner::checkUser(const userver::server::http::HttpRequest& request) const{

    using userver::storages::postgres::ClusterHostType;

    std::string_view login = request.GetArg("login");

    auto result = pg_cluster_->Execute(ClusterHostType::kSlave, checkQuery, login);

    if(result.IsEmpty()){
        return "account does not exist";
    }

    std::string_view password = request.GetArg("pass");

    if(result[0]["password"].As<std::string>() != password){
        return "wrong password";
    }

    return "ok!";
}

const userver::storages::postgres::Query registerQuery{"INSERT INTO user_db VALUES (DEFAULT, $1, $2, FALSE)",
                                                        userver::storages::postgres::Query::Name{"sample_reg"}};

std::string DBListner::registerUser(const userver::server::http::HttpRequest& request) const{

    using userver::storages::postgres::ClusterHostType;

    std::string_view login = request.GetArg("login");

    auto result = pg_cluster_->Execute(ClusterHostType::kSlave, checkQuery, login);

    if(!result.IsEmpty()){
        return "already exist";
    }

    std::string_view password = request.GetArg("pass");

    
    pg_cluster_->Execute(ClusterHostType::kMaster, registerQuery, login, password);

    return "registered";
}

const userver::storages::postgres::Query changeQuery{"UPDATE user_db SET password=$1 WHERE login=$2",
                                                        userver::storages::postgres::Query::Name{"sample_check"}};

std::string DBListner::changePassword(const userver::server::http::HttpRequest& request) const{

    using userver::storages::postgres::ClusterHostType;

    std::string_view login = request.GetArg("login");
    std::string_view password = request.GetArg("pass");

    pg_cluster_->Execute(ClusterHostType::kMaster, changeQuery, password, login);

    return "changed!";
}