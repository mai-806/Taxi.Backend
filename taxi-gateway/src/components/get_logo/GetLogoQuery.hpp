#pragma once 

#include <userver/clients/dns/component.hpp>
#include <userver/components/component.hpp>
#include <userver/components/minimal_server_component_list.hpp>
#include <userver/formats/bson/inline.hpp>
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/storages/mongo/component.hpp>
#include <string>
#include <string_view>

namespace GetLogo {

    class GetLogoQuery final : public server::handlers::HttpHandlerBase {
    public:
        static constexpr std::string_view kName = "handler-logo";

        GetLogoQuery(const components::ComponentConfig& config,
                const components::ComponentContext& context)
            : HttpHandlerBase(config, context),
            pool_(context.FindComponent<components::Mongo>("mongo-taxi").GetPool()) {}

        std::string HandleRequestThrow(
        const server::http::HttpRequest& request,
        server::request::RequestContext&) const override {}

    private:
        void InsertNewLogo(const server::http::HttpRequest& request) const;
        std::string ReturnLogo(const server::http::HttpRequest& request) const;

        storages::mongo::PoolPtr pool_;

    };
}


