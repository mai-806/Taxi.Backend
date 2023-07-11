#include <userver/components/minimal_server_component_list.hpp>
#include <userver/clients/dns/component.hpp>
#include <userver/clients/http/component.hpp>
#include <userver/server/handlers/ping.hpp>
#include <userver/server/handlers/tests_control.hpp>
#include <userver/testsuite/testsuite_support.hpp>
#include <userver/utils/daemon_run.hpp>

#include "components/hello.hpp"
#include "components/auth_check.hpp"
#include "components/reg_check.hpp"
#include "components/get_logo/GetLogoQuery.hpp"
#include "components/profile_photo/ProfilePhotoQuery.hpp"

int main(int argc, char* argv[]) {
  auto component_list = userver::components::MinimalServerComponentList()
                            .Append<userver::server::handlers::Ping>()
                            .Append<userver::components::TestsuiteSupport>()
                            .Append<userver::components::HttpClient>()
                            .Append<userver::clients::dns::Component>()
                            .Append<userver::server::handlers::TestsControl>()
                            .Append<ProfilePhoto::ProfilePhotoQuery>()
                            .Append<GetLogo::GetLogoQuery>();

  APIGateway::AppendHello(component_list);
  APIGateway::AppendAuth(component_list);
  APIGateay::AppendReg(component_list);

  return userver::utils::DaemonMain(argc, argv, component_list);
}
