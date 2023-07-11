#include "ProfilePhotoQuery.hpp"

ProfilePhoto::ProfilePhotoQuery::ProfilePhotoQuery(const components::ComponentConfig& config,
        const components::ComponentContext& context)
    : HttpHandlerBase(config, context),
    pool_(context.FindComponent<components::Mongo>("mongo-taxi").GetPool()) {}

std::string ProfilePhoto::ProfilePhotoQuery::HandleRequestThrow(
    const server::http::HttpRequest& request,
    server::request::RequestContext&) const override {
    if (request.GetMethod() == server::http::HttpMethod::kPost) {
        InsertNewPhoto(request);
        return {};
    } else {
        return ReturnPhoto(request);
    }
}

void ProfilePhoto::ProfilePhotoQuery::InsertNewPhoto(const server::http::HttpRequest& request) const {
    const auto& id = request.GetArg("id");
    const auto& photo = request.GetArg("photo");

    using formats::bson::MakeDoc;
    auto photos = pool_->GetCollection("ProfilePhotos");
    photos.InsertOne(MakeDoc("id", id, "photo", photo));
    request.SetResponseStatus(server::http::HttpStatus::kCreated);
}

std::string ProfilePhoto::ProfilePhotoQuery::ReturnPhoto(
    const server::http::HttpRequest& request) const {
    const auto& id = request.GetArg("id");

    using formats::bson::MakeDoc;
    auto photos = pool_->GetCollection("ProfilePhotos");
    auto cursor = photos.Find(MakeDoc("id", id));

    const auto profile_photo = cursor["photo"].As<std::string>();

    return profile_photo;
}

