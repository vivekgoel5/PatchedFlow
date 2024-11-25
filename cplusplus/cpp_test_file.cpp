#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <sqlite3.h>


template<typename T>
/**
 * Adds two values of type T together and returns the result.
 * 
 * @param a The first value to be added.
 * @param b The second value to be added.
 * @return The sum of a and b.
 */
T a_plus_b(T a, T b) {
    return a + b;
}


/**
 * Executes an SQL query on the specified SQLite database and returns the results.
 *
 * This function takes an SQLite database connection and a SQL query string,
 * prepares and executes the query, and then retrieves the results as a two-dimensional
 * vector of strings where each inner vector represents a row and each string in
 * the inner vector represents a column value.
 *
 * @param db A pointer to the SQLite database connection.
 * @param query The SQL query string to be executed.
 * @return A vector of vectors, where each inner vector represents a row of results from
 *         the executed query. Each element in the row is a string representation of a column value.
 *         The function returns an empty vector if preparation of the statement fails.
 */
std::vector<std::vector<std::string>> sqlite(sqlite3* db, const std::string& query) {
    std::vector<std::vector<std::string>> results;
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return results;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::vector<std::string> row;
        for (int i = 0; i < sqlite3_column_count(stmt); i++) {
            const unsigned char* text = sqlite3_column_text(stmt, i);
            if (text) {
                row.push_back(std::string(reinterpret_cast<const char*>(text)));
            } else {
                row.push_back("");
            }
        }
        results.push_back(row);
    }

    sqlite3_finalize(stmt);
    return results;
}


template<typename T, typename F>
/**
 * Compares two items by applying a key mapping function to each item and 
 * returning an integer based on their ordering.
 * 
 * @param key_map A function that extracts a comparable key from an item.
 * @param item1 The first item to be compared.
 * @param item2 The second item to be compared.
 * @return Returns -1 if item1 comes before item2, 1 if item1 comes after item2, 
 *         and 0 if they are considered equal based on the key mapping.
 */
int compare(F key_map, const T& item1, const T& item2) {
    auto val1 = key_map(item1);
    auto val2 = key_map(item2);

    if (val1 < val2) return -1;
    if (val1 > val2) return 1;
    return 0;
}


/**
 * Generates a random string of alphabets of the specified length.
 * The string includes both uppercase and lowercase alphabets.
 * 
 * @param length The length of the random alphabet string to be generated.
 * @return A string containing random alphabets of the specified length.
 */

std::string random_alphabets(int length) {
    static const std::string chars =
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    static std::random_device rd;
    static std::mt19937 generator(rd());
    static std::uniform_int_distribution<> distribution(0, chars.size() - 1);

    std::string result;
    result.reserve(length);

    for (int i = 0; i < length; ++i) {
        result += chars[distribution(generator)];
    }

    return result;
}