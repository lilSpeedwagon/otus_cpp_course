#include "queries.hpp"

#include <sstream>


namespace join_server::db::queries {

std::string CreateTableQuery(const models::TableSchema& meta) {
    std::stringstream ss;
    ss << "CREATE TABLE IF NOT EXISTS " << meta.name << " (";
    bool is_first = true;
    for (const auto& c : meta.columns) {
        if (is_first) {
            is_first = false;
        } else {
            ss << ", ";
        }
        ss << c.name << " " << c.type;
    }
    ss << ");";
    return ss.str();
}

std::string InsertQuery(const models::TableSchema& meta, const std::list<std::string>& row_values) {
    std::stringstream ss;
    ss << "INSERT INTO " << meta.name << " (";
    bool is_first = true;
    for (const auto& c : meta.columns) {
        if (is_first) {
            is_first = false;
        } else {
            ss << ",";
        }
        ss << c.name;
    }
    ss << ") VALUES (";
    is_first = true;
    for (const auto& v : row_values) {
        if (is_first) {
            is_first = false;
        } else {
            ss << ",";
        }
        ss << '"' << v << '"';
    }
    ss << ");";
    return ss.str();
}

std::string TruncateQuery(const db::models::TableSchema& meta) {
    std::stringstream ss;
    ss << "DELETE FROM " << meta.name << ";";
    return ss.str();
}

std::string JoinQuery(const db::models::TableSchema& meta_lhs,
                      const db::models::TableSchema& meta_rhs,
                      const std::string& on_lhs,
                      const std::string& on_rhs,
                      const std::list<std::string>& lhs_fields,
                      const std::list<std::string>& rhs_fields) {
    std::stringstream ss;
    ss << "SELECT lhs." << on_lhs;
    for (const auto& c : lhs_fields) {
        ss << ", " << "lhs." << c << " AS " << meta_lhs.name << "_" << c;
    }
    for (const auto& c : rhs_fields) {
        ss << ", " << "rhs." << c  << " AS " << meta_rhs.name << "_" << c;
    }

    ss << " FROM " << meta_lhs.name << " AS lhs";
    ss << " JOIN " << meta_rhs.name << " AS rhs";
    ss << " ON lhs." << on_lhs << " = rhs." << on_rhs << ";";
    return ss.str(); 
}

std::string JoinExcludeQuery(const db::models::TableSchema& meta_lhs,
                             const db::models::TableSchema& meta_rhs,
                             const std::string& on_lhs,
                             const std::string& on_rhs,
                             const std::list<std::string>& lhs_fields,
                             const std::list<std::string>& rhs_fields) {
    std::stringstream ss;
    ss << "SELECT lhs." << on_lhs;
    for (const auto& c : lhs_fields) {
        ss << ", " << "lhs." << c << " AS " << meta_lhs.name << "_" << c;
    }
    for (const auto& c : rhs_fields) {
        ss << ", " << "rhs." << c  << " AS " << meta_rhs.name << "_" << c;
    }
    ss << " FROM " << meta_lhs.name << " AS lhs";
    ss << " LEFT JOIN " << meta_rhs.name << " AS rhs";
    ss << " ON lhs." << on_lhs << " = rhs." << on_rhs;
    ss << " WHERE rhs." << on_rhs << " IS null";

    ss << " UNION ";

    ss << "SELECT rhs." << on_rhs;
    for (const auto& c : lhs_fields) {
        ss << ", " << "lhs." << c << " AS " << meta_lhs.name << "_" << c;
    }
    for (const auto& c : rhs_fields) {
        ss << ", " << "rhs." << c  << " AS " << meta_rhs.name << "_" << c;
    }
    ss << " FROM " << meta_lhs.name << " AS lhs";
    ss << " RIGHT JOIN " << meta_rhs.name << " AS rhs";
    ss << " ON lhs." << on_lhs << " = rhs." << on_rhs;
    ss << " WHERE lhs." << on_lhs << " IS null";

    // TODO ????

    return ss.str(); 
}

} // namespace join_server::db::queries
