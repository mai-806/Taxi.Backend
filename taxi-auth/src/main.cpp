#include <userver/clients/http/component.hpp>
#include <userver/components/minimal_server_component_list.hpp>
#include <userver/server/handlers/ping.hpp>
#include <userver/server/handlers/tests_control.hpp>
#include <userver/testsuite/testsuite_support.hpp>
#include <userver/utils/daemon_run.hpp>
#include <userver/clients/dns/component.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/component.hpp>

#include "hello.hpp"
#include "./authorization/AuthorizationQuery.cpp"
#include "./registration/RegistrationQuery.cpp"
#include "./recovery/PassRecovery.cpp"
#include "DBListner.cpp"

int main(int argc, char* argv[]) {
  auto component_list = userver::components::MinimalServerComponentList()
                            .Append<userver::components::Postgres>("users-db")
                            .Append<DataBaseListner::DBListner>()
                            .Append<userver::server::handlers::Ping>()
                            .Append<userver::components::TestsuiteSupport>()
                            .Append<userver::components::HttpClient>()
                            .Append<userver::clients::dns::Component>()
                            .Append<userver::server::handlers::TestsControl>()
                            .Append<Authorization::AuthQuery>()
                            .Append<Recovery::PassRecQuery>()
                            .Append<Registration::RegQuery>();

  return userver::utils::DaemonMain(argc, argv, component_list);
}
