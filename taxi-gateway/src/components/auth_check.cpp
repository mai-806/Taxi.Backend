#include "auth_check.hpp"

#include <fmt/format.h>

#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/clients/http/client.hpp>
#include <userver/clients/http/component.hpp>
#include <userver/components/component_context.hpp>
#include <userver/formats/json.hpp>
#include <userver/components/minimal_server_component_list.hpp>

namespace APIGateway {

namespace {



class AuthCheck final : public userver::server::handlers::HttpHandlerBase {
public:
  using HttpHandlerBase::HttpHandlerBase;

  static constexpr std::string_view kName = "handler-auth-check";
  userver::components::HttpClient& http_client;

  AuthCheck(const userver::components::ComponentConfig& config,
                     const userver::components::ComponentContext& context):
                     HttpHandlerBase(config, context), 
                     http_client(context.FindComponent<userver::components::HttpClient>()) {}

  std::string HandleRequestThrow(
      const userver::server::http::HttpRequest& request,
      userver::server::request::RequestContext&) const override {

    std::string reqBody = request.RequestBody();
    
    int flag = APIGateway::checkCorrectly(reqBody);

    switch (flag) {
        case 1:
            return "Incorrect email";

        case 2: 
            return "Incorrect password!";

        case 3:
            return "Incorrect driver flag!";                
    }

    auto& client = http_client.GetHttpClient();
    userver::formats::json::Value json = APIGateway::hashPassword(reqBody);

    const auto& response = client.CreateRequest()
                                .post("http://localhost:8080/password", userver::formats::json::ToString(json))
                                .timeout(std::chrono::seconds(1))
                                .perform();

    response->raise_for_status();
    return response->body();
  }
};
} // namespace

int checkCorrectly(std::string request) {
    userver::formats::json::Value json = userver::formats::json::FromString(request);

    const auto login = json["login"].As<std::string>();
    const auto password = json["password"].As<std::string>();
    const auto driver_flag = json["isDriver"].As<bool>();

    if (login.size() <= 8 && login.size() >= 32) return 1;
    if (password.size() <= 8 && password.size() >= 32) return 2;
    // if (driver_flag == "Incorrect") return 3;

    return 0;
}

userver::formats::json::Value hashPassword(std::string str_json) {
  userver::formats::json::Value json = userver::formats::json::FromString(str_json);

  auto password = json["password"].As<std::string>();
  const auto login = json["login"].As<std::string>();
  const auto isDriver = json["isDriver"].As<bool>();
  size_t hash_password = std::hash<std::string>{}(password);

  userver::formats::json::Value new_json = userver::formats::json::FromString(
    "{ \n \"login\": \"%s\",\npassword\": \"%s\"\n\"isDriver\": %ld\n }",
    login,
    hash_password,
    isDriver
  );

  // userver::formats::json::Value new_json = userver::formats::json::FromString(R"({
  //   "login": \"$1,
  //   "password": ""
  // })");

  return new_json;
}

void AppendAuth(userver::components::ComponentList &component_list) {
  component_list.Append<AuthCheck>();
}

} // namespace APIGateway