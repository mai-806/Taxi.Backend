#pragma once

#include <userver/components/component.hpp>
#include <userver/components/loggable_component_base.hpp>
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/http/url.hpp>
#include <string>
#include <string_view>

class AuthQuery final : public userver::components::LoggableComponentBase{
public:
    static constexpr std::string_view kName = "auth-query";

    AuthQuery(const userver::components::ComponentConfig& config,
              const userver::components::ComponentContext& context);
    
    std::string makeAuthQuery(const std::pair<std::string, std::string>&);

};