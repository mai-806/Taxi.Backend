#include "RegistrationQuery.hpp"

RegQuery::RegQuery(const userver::components::ComponentConfig& config,
                   const userver::components::ComponentContext& context):
                   HttpHandlerJsonBase (config, context),
                   httpClient{context.FindComponent<userver::components::HttpClient>().GetHttpClient()}{}

userver::formats::json::Value RegQuery::HandleRequestJsonThrow(const userver::server::http::HttpRequest &request, 
                                                               const userver::formats::json::Value &request_json,
                                                               userver::server::request::RequestContext &context) const{

    LOG_INFO() << "parsing request JSON body from: " + request.GetUrl();
    const std::vector<std::string> userData = RegQuery::parseJsonData(request_json);

    LOG_INFO() << "making HTTP query to Postgre`s DB";
    std::string DBresponse = RegQuery::makeRegQuery(userData, httpClient);

    std::stringstream data(DBresponse);

    LOG_INFO() << "Serializing data";
    userver::formats::json::Value responseJson = RegQuery::Serialize(data);

    return responseJson;
}

std::string RegQuery::makeRegQuery(const std::vector<std::string>& userData, userver::clients::http::Client& httpclient){
    std::pair<std::string, std::string> login("login", userData[0]);
    std::pair<std::string, std::string> password("pass", userData[1]);
    std::pair<std::string, std::string> is_driver("driver", userData[2]);

    LOG_INFO() << "Creating URL";
    const auto url = userver::http::MakeUrl(pgDBIP, { login, password, is_driver });

    LOG_INFO() << "Creating HTTP request";
    auto request = httpclient.CreateRequest()
                             .get(url)
                             .retry(2)
                             .timeout(std::chrono::seconds{1});

    LOG_INFO() << "Sending query to " + pgDBIP;
    auto response = request.perform();

    return response->body();
}

std::vector<std::string> RegQuery::parseJsonData(const userver::formats::json::Value &request_json){
    std::string login = request_json["email"].As<std::string>();
    std::string password = request_json["password"].As<std::string>();
    std::string driver = request_json["is_Driver"].As<std::string>();

    return std::vector<std::string>{login, password, driver};
}

userver::formats::json::Value RegQuery::Serialize(std::stringstream& stream){
    userver::formats::json::ValueBuilder builder;
    std::string status;
    std::string id;
    stream >> status >> id;
    builder["status"] = status;
    builder["id"] = id;

    return builder.ExtractValue();
}

