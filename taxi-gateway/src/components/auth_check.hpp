#pragma once

#include <string>
#include <string_view>

#include <userver/components/component_list.hpp>

namespace APIGateway {

int checkCorrectly(std::string name);

void AppendAuth(userver::components::ComponentList &component_list);

} // namespace APIGateway
