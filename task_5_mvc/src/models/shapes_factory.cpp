#include "shapes_factory.hpp"


namespace mvc::shapes {

ShapesFactory::ShapesFactory() {
    Init();
}

ShapesFactory::~ShapesFactory() {}

ShapePtr ShapesFactory::BuildShape(
    const std::string& shape_name, const std::vector<float>& params) const {
    if (params.size() != GetParams(shape_name).size()) {
        throw std::logic_error("invalid params number");
    }

    ShapePtr result_ptr = nullptr;
    if (shape_name == "rect") {
        Point lbc{params[0], params[1]};
        Point rtc{params[2], params[3]};
        auto ptr = std::make_shared<Rectangle>(lbc, rtc);
        result_ptr = std::move(ptr);
    } else if (shape_name == "triangle") {
        Point fp{params[0], params[1]};
        Point sp{params[2], params[3]};
        Point tp{params[4], params[5]};
        auto ptr = std::make_shared<Triangle>(fp, sp, tp);
        result_ptr = std::move(ptr);
    } else if (shape_name == "circle") {
        Point center{params[0], params[1]};
        float radius = params[2];
        auto ptr = std::make_shared<Circle>(center, radius);
        result_ptr = std::move(ptr);
    }
    return result_ptr;
}

std::vector<std::string> ShapesFactory::GetParams(
    const std::string& shape_name) const {
    if (auto it = points_map_.find(shape_name);
        it != points_map_.end()) {
        return it->second;
    }
    throw std::runtime_error("unknown shape");
}

void ShapesFactory::Init() {
    points_map_ = {
        {"rect", {"left bottom corner X",
                    "left bottom corner Y",
                    "right top corner X",
                    "right top corner Y"}},
        {"triangle", {"first point x",
                        "first point y",
                        "second point x",
                        "second point y",
                        "third point x",
                        "third point y"}},
        {"circle", {"center x",
                    "center y",
                    "radius"}}
    };
}

} // namespace mvc::shapes
