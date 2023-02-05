#include <iostream>

#include <boost/algorithm/string.hpp>
#include <boost/program_options.hpp>
#include <boost/tokenizer.hpp>

#include <hash/factory.hpp>
#include <file_comparator/file_comparator.hpp>
#include <file_reader/factory.hpp>
#include <file_scanner/file_scanner.hpp>


namespace {

namespace boost_options = boost::program_options;

void ShowHelp(const boost_options::options_description& options) {
    std::cout << "bayan: search for duplicate files by their hashes\n" << options;
}

boost_options::options_description PrepareOptions() {
    boost_options::options_description options("options");
    options.add_options()
        ("help", "show help message")
        ("hash", boost_options::value<std::string>()->default_value("default"),
         "defines hash algorithm (define, rs)")
        ("block-size", boost_options::value<size_t>()->default_value(1024),
         "size of file block to read")
        ("include", boost_options::value<std::string>()->default_value("./"), 
         "list of directories to search delimited by space")
        ("exclude", boost_options::value<std::string>()->default_value(""), 
         "list of directories to exclude from search delimited by space")
        ("wildcards", boost_options::value<std::string>()->default_value(".*"),
         "file name wildcards separated by space")
        ("depth", boost_options::value<size_t>()->default_value(999999),
         "depth of search")
        ("min-size", boost_options::value<size_t>()->default_value(0),
         "minimal file size to compare");
    return options;
}

boost_options::variables_map ParseCommandLine(
    const boost_options::options_description& options, int argc, char** argv) {
    boost_options::variables_map variables;
    boost_options::store(boost_options::parse_command_line(argc, argv, options), variables);
    boost_options::notify(variables);
    return variables;
}

std::vector<std::string> Tokenize(const std::string& data) {
    std::vector<std::string> result;
    boost::algorithm::split(result, data, boost::is_any_of(" "));
    return result;
}

} // namespace

int main(int argc, char** argv) {
    auto options = PrepareOptions();
    auto variables = ParseCommandLine(options, argc, argv);
    if (variables.count("help")) {
        ShowHelp(options);
        return 0;
    }

    auto include = Tokenize(variables["include"].as<std::string>());
    auto exclude = Tokenize(variables["exclude"].as<std::string>());
    auto wildcards = Tokenize(variables["wildcards"].as<std::string>());

    auto hash_type = variables["hash"].as<std::string>();
    auto block_size = variables["block-size"].as<size_t>();
    auto depth = variables["depth"].as<size_t>();
    auto min_size = variables["min-size"].as<size_t>();

    bayan::file::FileScanner scanner(include, exclude, wildcards, depth, min_size);
    scanner.Scan();
    auto files = scanner.GetScannedFiles();

    bayan::hash::HashProcessorFactory hash_factory(hash_type);
    bayan::file::FileHashReaderFactory hash_reader_factory(std::move(hash_factory), block_size);
    bayan::file::FileComparator comparator(std::move(hash_reader_factory));
    auto result = comparator.Compare(files);

    for (const auto& [_, file_group] : result) {
        for (const auto& path : file_group) {
            std::cout << path << '\n';
        }
        std::cout << '\n';
    }

    return 0;
}
