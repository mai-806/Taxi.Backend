#include "DBListner.hpp"

DBListner::DBListner(const userver::components::ComponentConfig& config,
                     const userver::components::ComponentContext& context):
                     HttpHandlerBase(config, context),
                     pg_cluster_(context.FindComponent<userver::components::Postgres>("users-db").GetCluster()){

    
 
    using userver::storages::postgres::ClusterHostType;
    LOG_INFO() << "Creating DB Table";
    pg_cluster_->Execute(ClusterHostType::kMaster, userver::formats::postgre::response::kCreateTable);

}


std::string DBListner::HandleRequestThrow(const userver::server::http::HttpRequest& request,
                              userver::server::request::RequestContext& context) const {

    auto method = request.GetMethod();

    LOG_INFO() << "Looking for HTTP method:";
    switch(method){
        case(userver::server::http::HttpMethod::kGet):
            LOG_INFO() << "GET";
            return checkUser(request);

        case(userver::server::http::HttpMethod::kPost):
            LOG_INFO() << "POST";
            return registerUser(request);

        case(userver::server::http::HttpMethod::kDelete):
            LOG_INFO() << "DELETE";
            return changePassword(request);

        default:
            LOG_DEBUG() << "no method founded";
            return "method not found";
    }

}

std::string DBListner::checkUser(const userver::server::http::HttpRequest& request) const{

    using userver::storages::postgres::ClusterHostType;

    LOG_INFO() << "Getting data from request";
    std::string_view login = request.GetArg("login");
    std::string_view password = request.GetArg("pass");

    LOG_INFO() << "Checks for user";
    auto result = pg_cluster_->Execute(ClusterHostType::kSlave, userver::formats::postgre::response::checkQuery, login);

    if(result.IsEmpty()){
        LOG_DEBUG() << "no user founded";
        return "account does not exist";
    }

    if(result[0]["password"].As<std::string>() != password){
        LOG_DEBUG() << "wrong password founded";
        return "wrong password";
    }

    if(result[0]["driver"].As<bool>()){
        LOG_INFO() << "driver founded";
        return "driver " + result[0]["id"].As<std::string>();
    }
    else if(result[0]["admin"].As<bool>()){
        LOG_INFO() << "admin founded";
        return "admin " + result[0]["id"].As<std::string>();
    }

    LOG_INFO() << "user founded";
    return "user " + result[0]["id"].As<std::string>();

}

std::string DBListner::registerUser(const userver::server::http::HttpRequest& request) const{

    using userver::storages::postgres::ClusterHostType;

    LOG_INFO() << "Getting data from request";
    std::string_view login = request.GetArg("login");
    std::string_view password = request.GetArg("pass");
    std::string_view driver = request.GetArg("driver");
    std::string_view admin = request.GetArg("admin");

    LOG_INFO() << "Checks for user";
    auto result = pg_cluster_->Execute(ClusterHostType::kSlave, userver::formats::postgre::response::checkQuery, login);

    if(!result.IsEmpty()){
        LOG_DEBUG() << "user already exist";
        return "already exist";
    }
    
    LOG_INFO() << "Registering user";
    pg_cluster_->Execute(ClusterHostType::kMaster, userver::formats::postgre::response::registerQuery, login, password, driver, admin);

    return "registered";
}

std::string DBListner::changePassword(const userver::server::http::HttpRequest& request) const{

    using userver::storages::postgres::ClusterHostType;

    LOG_INFO() << "Getting data from request";
    std::string_view login = request.GetArg("login");
    std::string_view password = request.GetArg("pass");

    LOG_INFO() << "Checks for user";
    auto result = pg_cluster_->Execute(ClusterHostType::kSlave, userver::formats::postgre::response::checkQuery, login);

    if(result.IsEmpty()){
        LOG_DEBUG() << "no user founded";
        return "account does not exist";
    }

    LOG_INFO() << "Changing password";
    pg_cluster_->Execute(ClusterHostType::kMaster, userver::formats::postgre::response::changeQuery, password, login);

    return "ok!";
}