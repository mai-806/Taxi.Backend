#include "auth_check.hpp"

#include <fmt/format.h>

#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/formats/json.hpp>

namespace APIGateway {

namespace {

class AuthCheck final : public userver::server::handlers::HttpHandlerBase {
public:
  static constexpr std::string_view kName = "handler-auth-check";

  using HttpHandlerBase::HttpHandlerBase;

  std::string HandleRequestThrow(
      const userver::server::http::HttpRequest &request,
      userver::server::request::RequestContext &) const override {
    
    int flag = APIGateway::checkCorrectly(request.RequestBody());

    switch (flag) {
        case 1:
            return "Incorrect email";

        case 2: 
            return "Incorrect password!";

        case 3:
            return "Incorrect driver flag!";

        case 0:
            return "All data is correct";

        default:
            return "Unknown json parametr";                
    }

    // send request into kafka
    // ...

    // maybe caching fyles
    // ...

    // get response from kafka
    // ...
  }
};
} // namespace

int checkCorrectly(std::string request) {
    userver::formats::json::Value json = userver::formats::json::FromString(request);

    const auto login = json["login"].As<std::string>();
    const auto password = json["password"].As<std::string>();
    const auto driver_flag = json["driver_flag"].As<bool>();

    if (login.size() <= 8 && login.size() >= 32) return 1;
    if (password.size() <= 8 && password.size() >= 32) return 2;
    // if (driver_flag == "Incorrect") return 3;

    return 0;

}

void AppendAuth(userver::components::ComponentList &component_list) {
  component_list.Append<AuthCheck>();
}

} // namespace APIGateway