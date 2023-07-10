#include "PassRecovery.hpp"

PassRecQuery::PassRecQuery(const userver::components::ComponentConfig& config,
                           const userver::components::ComponentContext& context):
                           HttpHandlerJsonBase (config, context),
                           httpClient{context.FindComponent<userver::components::HttpClient>().GetHttpClient()}{}

userver::formats::json::Value PassRecQuery::HandleRequestJsonThrow(const userver::server::http::HttpRequest &request, 
                                                                   const userver::formats::json::Value &request_json,
                                                                   userver::server::request::RequestContext &context) const{

    LOG_INFO() << "parsing request JSON body from: " + request.GetUrl();
    const std::string user_login = PassRecQuery::parseJsonData(request_json);

    LOG_INFO() << "making HTTP query to Postgre`s DB";
    std::string DBresponse = PassRecQuery::makePassRecQuery(user_login, httpClient);

    std::stringstream data(DBresponse);

    LOG_INFO() << "Serializing data";
    userver::formats::json::Value responseJson = PassRecQuery::Serialize(data);

    return responseJson;
}

static std::string PassRecQuery::makePassRecQuery(const std::string& user_login, 
                                                  userver::clients::http::Client& httpclient){

    std::string new_pass = PassRecQuery::generatePass();

    std::pair<std::string, std::string> login("login", user_login);
    std::pair<std::string, std::string> password("pass", new_pass);

    LOG_INFO() << "Sending new passsword to email";
    PassRecQuery::sendPassToEmail(user_login, new_pass);

    LOG_INFO() << "Creating URL";
    const auto url = userver::http::MakeUrl(pgDBIP, {login, password});

    LOG_INFO() << "Creating HTTP request";
    auto request = httpclient.CreateRequest()
                             .get(url)
                             .retry(2)
                             .timeout(std::chrono::seconds{1});

    LOG_INFO() << "Sending query to " + pgDBIP;
    auto response = request.perform();

    return response->body();
}

static std::string PassRecQuery::parseJsonData(const userver::formats::json::Value &request_json){
    std::string login = request_json["email"].As<std::string>();

    return login;
}

static userver::formats::json::Value PassRecQuery::Serialize(std::stringstream& stream){
    userver::formats::json::ValueBuilder builder;
    std::string status;
    std::string id;
    stream >> status >> id;
    builder["status"] = status;
    builder["id"] = id;

    return builder.ExtractValue();
}

static std::string PassRecQuery::generatePass(){
    std::string password;

    std::uniform_int_distribution<int> dist(0, set_symbols.length() - 1);
    std::uniform_int_distribution<size_t> len(8, 20); 
    size_t pass_len = len(rng);

    for (size_t i = 0; i != pass_len; ++i){
        password += set_symbols[dist(rng)];
    }

    std::string result;

    for (char c: password){
        if (c == '&' || c == '$' || c == '>' || c == '<' || c == '#') result += '\\';
        result += c;
    }

    return result;
}

void PassRecQuery::sendPassToEmail(const std::string& recipient_mail, const std::string& new_pass){
    std::string command = "python3 PassSender.py -e " + recipient_mail + " -p " + new_pass;
    std::system(command.c_str());

    return;
}