#include "auth_check.cpp"
#include "reg_check.hpp"


class RegCheck final : public userver::server::handlers::HttpHandlerBase {
public:
    using HttpHandlerBase::HttpHandlerBase;

    static constexpr std::string_view kName = "handler-reg-check";

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

        return "All data is correct";
    }
};

void AppendReg(userver::components::ComponentList& component_list) {
    component_list.Append<RegCheck>();
}