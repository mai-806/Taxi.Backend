#pragma once

#include <userver/components/component.hpp>
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/component.hpp>


#include <string>
#include <string_view>
#include <vector>

class DBListner final : public userver::server::handlers::HttpHandlerBase {
public:
    static constexpr std::string_view kName = "dblistner";

    DBListner(const userver::components::ComponentConfig& config,
              const userver::components::ComponentContext& context);

    std::string HandleRequestThrow(const userver::server::http::HttpRequest&,
                                   userver::server::request::RequestContext&) const override;

    std::string checkUser(const userver::server::http::HttpRequest&) const;
    std::string registerUser(const userver::server::http::HttpRequest&) const;
    std::string changePassword(const userver::server::http::HttpRequest&) const;

private:
    userver::storages::postgres::ClusterPtr pg_cluster_;
};