#pragma once

#include <userver/components/component.hpp>
#include <userver/clients/http/component.hpp>
#include <userver/components/minimal_server_component_list.hpp>
#include <userver/components/loggable_component_base.hpp>
#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/http/url.hpp>

#include <cstdlib>
#include <random>
#include <string>
#include <string_view>
#include <sstream>
#include <vector>

namespace Recovery::vars{

    const std::string pgDBIP = "http://127.0.0.1:8080/pgsql";
    const std::string set_symbols = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*_+{}[]?=/";
    std::random_device rd{};
    std::mt19937_64 rng{ rd() };

}

namespace Recovery{

    class PassRecQuery final: public userver::server::handlers::HttpHandlerJsonBase{
    public:
        static constexpr std::string_view kName = "recovery-query";

        PassRecQuery(const userver::components::ComponentConfig&,
                     const userver::components::ComponentContext&);

        userver::formats::json::Value HandleRequestJsonThrow(const userver::server::http::HttpRequest&, 
                                                             const userver::formats::json::Value&,
                                                             userver::server::request::RequestContext&) const override;

        static std::string makePassRecQuery(const std::string&, userver::clients::http::Client&);
        static std::string parseJsonData(const userver::formats::json::Value&);
        static userver::formats::json::Value Serialize(std::stringstream&);

        static std::string generatePass();
        static void sendPassToEmail(const std::string&, const std::string&);

    private:

        userver::clients::http::Client& httpClient;

    };

}