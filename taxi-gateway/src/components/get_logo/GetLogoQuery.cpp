#include "GetLogoQuery.hpp"

GetLogo::GetLogoQuery::GetLogoQuery(const components::ComponentConfig& config,
        const components::ComponentContext& context)
    : HttpHandlerBase(config, context),
    pool_(context.FindComponent<components::Mongo>("mongo-taxi").GetPool()) {}

std::string GetLogo::GetLogoQuery::HandleRequestThrow(
    const server::http::HttpRequest& request,
    server::request::RequestContext&) const override {
    if (request.GetMethod() == server::http::HttpMethod::kPost) {
        InsertNewLogo(request);
        return {};
    } else {
        return ReturnLogo(request);
    }
}

void GetLogo::GetLogoQuery::InsertNewLogo(const server::http::HttpRequest& request) const {
    const auto& logo = request.GetArg("logo");

    using formats::bson::MakeDoc;
    auto logo_coll = pool_->GetCollection("Logotype");
    logo_coll.InsertOne(MakeDoc("logo", logo));
    request.SetResponseStatus(server::http::HttpStatus::kCreated);
}

std::string GetLogo::GetLogoQuery::ReturnLogo(
    const server::http::HttpRequest& request) const {

    auto logo_coll = pool_->GetCollection("Logotype");

    const auto logo = logo_coll["logo"].As<std::string>();

    return logo;
}

