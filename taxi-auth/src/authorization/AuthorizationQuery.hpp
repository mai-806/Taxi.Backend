#pragma once

#include <userver/components/component.hpp>
#include <userver/clients/http/component.hpp>
#include <userver/components/minimal_server_component_list.hpp>
#include <userver/components/loggable_component_base.hpp>
#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/http/url.hpp>
#include <string>
#include <string_view>
#include <sstream>
#include <vector>

static const std::string pgDBIP = "127.0.0.1:8080/pgsql";

class AuthQuery final : public userver::server::handlers::HttpHandlerJsonBase {
public:
    static constexpr std::string_view kName = "auth-query";

    AuthQuery(const userver::components::ComponentConfig& config,
              const userver::components::ComponentContext& context);
    
    userver::formats::json::Value HandleRequestJsonThrow(const userver::server::http::HttpRequest &request, 
                                                const userver::formats::json::Value &request_json,
                                                userver::server::request::RequestContext &context) const override;

    static std::string makeAuthQuery(const std::vector<std::string>&, userver::clients::http::Client&);

    static std::vector<std::string> parseJsonData(const userver::formats::json::Value &request_json);

    static userver::formats::json::Value Serialize(std::stringstream& stream);

private:

    userver::clients::http::Client& httpClient;

};