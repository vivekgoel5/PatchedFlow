#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <sqlite3.h>


template<typename T>
/**
 * Adds two values of the same type together and returns the result.
 * 
 * @param a The first value to be added.
 * @param b The second value to be added.
 * @return The sum of the two values.
 */

T a_plus_b(T a, T b) {
    return a + b;
}


/**
 * Executes an SQL query on the provided SQLite database and retrieves the results.
 * The results are returned as a vector of vectors of strings, where each vector
 * represents a row in the result set, and each string represents a column in that row.
 * 
 * @param db A pointer to an sqlite3 object representing the open SQLite database on which the query should be executed.
 * @param query A string representing the SQL query to be executed on the database.
 * @return A vector of vectors of strings containing the results of the query. 
 * Each inner vector represents a row of the query result, with strings representing the column values.
 * If the query fails, an empty vector is returned.
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
 * Compares two items using a key mapping function.
 * The function uses the provided key_map to retrieve values from the items and then compares these values.
 * Returns -1 if the first item is less than the second, 1 if greater, and 0 if they are equal.
 * 
 * @param key_map A function that maps items to comparable values.
 * @param item1 The first item to compare.
 * @param item2 The second item to compare.
 * @return An integer indicating the comparison result: -1, 0, or 1.
 */
int compare(F key_map, const T& item1, const T& item2) {
    auto val1 = key_map(item1);
    auto val2 = key_map(item2);

    if (val1 < val2) return -1;
    if (val1 > val2) return 1;
    return 0;
}


/**
 * Generates a random string composed of both lowercase and uppercase alphabets.
 * 
 * @param length The desired length of the generated random string.
 * @return A random string of specified length consisting of alphabetical characters.
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