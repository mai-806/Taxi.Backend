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

namespace Authorization::vars{

    const std::string pgDBIP = "http://127.0.0.1:8080/pgsql";

}

namespace Authorization{

    class AuthQuery final : public userver::server::handlers::HttpHandlerJsonBase {
    public:
        static constexpr std::string_view kName = "auth-query";

        AuthQuery(const userver::components::ComponentConfig&,
                  const userver::components::ComponentContext&);
    
        userver::formats::json::Value HandleRequestJsonThrow(const userver::server::http::HttpRequest&, 
                                                        const userver::formats::json::Value&,
                                                        userver::server::request::RequestContext&) const override;

        static std::string makeAuthQuery(const std::vector<std::string>&, userver::clients::http::Client&);
        static std::vector<std::string> parseJsonData(const userver::formats::json::Value&);
        static userver::formats::json::Value Serialize(std::stringstream&);

    private:

        userver::clients::http::Client& httpClient;

    };

}