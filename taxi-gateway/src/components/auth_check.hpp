#pragma once

#include <string>
#include <string_view>

#include <userver/components/component_list.hpp>
#include <userver/formats/json.hpp>

namespace APIGateway {

int checkCorrectly(std::string name);

userver::formats::json::Value hashPassword(std::string str_json);

void AppendAuth(userver::components::ComponentList &component_list);

} // namespace APIGateway
