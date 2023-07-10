#pragma once

#include <string>
#include <string_view>

#include <userver/components/component_list.hpp>

namespace APIGateway {

void AppendReg(userver::components::ComponentList& component_list);

} // namespace APIGateway