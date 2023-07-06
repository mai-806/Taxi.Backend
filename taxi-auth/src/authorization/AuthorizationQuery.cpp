#include "AuthorizationQuery.hpp"

AuthQuery::AuthQuery(const userver::components::ComponentConfig& config,
                     const userver::components::ComponentContext& context):
                     LoggableComponentBase(config, context){}

std::string AuthQuery::makeAuthQuery(const std::pair<std::string, std::string>& loginAndPass){

    return userver::http::MakeUrl("127.0.0.1:8080/pgsql", {loginAndPass});
}