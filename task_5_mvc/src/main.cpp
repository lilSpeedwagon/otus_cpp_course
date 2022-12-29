#include <iostream>
#include <memory>

#include <editor/editor.hpp>
#include <models/document.hpp>
#include <models/point.hpp>
#include <models/shape.hpp>
#include <models/shapes_factory.hpp>
#include <render/display_render.hpp>
#include <storage/file_storage.hpp>


std::unique_ptr<mvc::controller::Editor> InitEditor() {
    auto storage_ptr = std::make_shared<mvc::documents::FileStorage>();
    return std::make_unique<mvc::controller::Editor>(storage_ptr);
}

void PrintHelp() {
    std::cout << "Commands:\n"
        << "TODO\n\n";
}

float GetParam(const std::string& name) {
    float param;
    std::cout << name << ": ";
    std::cin >> param;
    return param;
}

int main() {
    auto editor_ptr = InitEditor();
    mvc::shapes::ShapesFactory factory;
    bool exit = false;

    std::unordered_map<std::string, std::function<void(void)>> commands = {
        {"create", [&editor_ptr]() {
            editor_ptr->CreateDocument();
            auto name = editor_ptr->GetCurrentDocumentName();
            std::cout << "New file is created as \"" << name.value() << "\".\n";
        }},
        {"open", [&editor_ptr]() {
            std::cout << "Document name: ";
            std::string name;
            std::cin >> name;
            if (editor_ptr->ImportDocument(name)) {
                std::cout << "Document \"" << name << "\" is opened\n";
            } else {
                std::cout << "Document \"" << name << "\" is not found\n";
            }
        }},
        {"save", [&editor_ptr]() {
            std::cout << "Document name: ";
            std::string name;
            std::cin >> name;
            if (editor_ptr->ExportDocument(name)) {
                std::cout << "Document \"" << name << "\" is saved\n";
            } else {
                std::cout << "No opened document\n";
            }
        }},
        {"close", [&editor_ptr]() {
            bool is_closed = editor_ptr->CloseDocument();
            if (is_closed) {
                std::cout << "Document is closed.\n";
            } else {
                std::cout << "No opened documents to close.\n";
            }
        }},
        {"add-shape", [&editor_ptr, &factory]() {
            auto doc = editor_ptr->GetCurrentDocument();
            if (doc == nullptr) {
                std::cout << "No opened document\n";
                return;
            }
            
            std::cout << "Shape name: ";
            std::string name;
            std::cin >> name;
            std::vector<std::string> param_names;
            try {
                param_names = factory.GetParams(name);
            } catch (const std::runtime_error& ex) {
                std::cout << "Error: " << ex.what() << "\n";
                return;
            }

            std::vector<float> params;
            for (const auto& param_name : param_names) {
                auto param = GetParam(param_name);
                params.push_back(param);
            }
            mvc::shapes::ShapePtr shape_ptr;
            try {
                shape_ptr = factory.BuildShape(name, params);
            } catch(const std::runtime_error& ex) {
                std::cout << "Error: " << ex.what() << "\n";
                return;
            }
            auto shape_id = doc->AddShape(shape_ptr);
            std::cout << "Shape " << name 
                      << " with id=" << shape_id.GetUnderlying() << " is added\n";
        }},
        {"delete-shape", [&editor_ptr]() {
            auto doc = editor_ptr->GetCurrentDocument();
            if (doc == nullptr) {
                std::cout << "No opened document\n";
                return;
            }
            
            mvc::shapes::ShapeId::underlying_type shape_id_value;
            std::cout << "Shape id: ";
            std::cin >> shape_id_value;
            mvc::shapes::ShapeId shape_id{shape_id_value};
            auto is_deleted = doc->DeleteShape(shape_id);
            if (is_deleted) {
                std::cout << "Shape is deleted\n";
            } else {
                std::cout << "Shape with id=" << shape_id_value << " is not found\n";
            }
        }},
        {"help", []() {
            PrintHelp();
        }},
        {"exit", [&exit]() {
            exit = true;
        }},
    };

    std::cout << "Welcome to Cool Vector Graphic Editor 1.0. Type help to show list of available commands.\n";
    while (!exit) {
        auto current_document_name_opt = editor_ptr->GetCurrentDocumentName();
        std::cout << "[" << current_document_name_opt.value_or("-") << "]: ";
        std::string command_input;
        std::cin >> command_input;
        if (auto it = commands.find(command_input);
            it != commands.end()) {
            it->second();
        } else {
            std::cout << "Unknown command, type \"help\" for instructions\n"; 
        }
        std::cout << std::endl;
    }

    return 0;
}
