#include <iostream>
#include <sqlite3.h>

int main() {
    // Open SQLite database
    sqlite3 *db;
    int rc = sqlite3_open("sj.db", &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }

    // Execute SQL query to calculate student-wise totals and averages
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, "SELECT name, math, english FROM score", -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return 1;
    }

    int mathTotal = 0;
    int englishTotal = 0;
    int numStudents = 0;

    // Iterate over query results to calculate subject-wise totals
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        const char *name = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
        int mathScore = sqlite3_column_int(stmt, 1);
        int englishScore = sqlite3_column_int(stmt, 2);
        mathTotal += mathScore;
        englishTotal += englishScore;
        numStudents++;
    }

    // Finalize the prepared statement
    sqlite3_finalize(stmt);

    // Calculate subject-wise averages
    double mathAverage = static_cast<double>(mathTotal) / numStudents;
    double englishAverage = static_cast<double>(englishTotal) / numStudents;

    // Print results
    std::cout << "Subject-wise Averages:" << std::endl;
    std::cout << "Math: " << mathAverage << std::endl;
    std::cout << "English: " << englishAverage << std::endl;

    // Close the database
    sqlite3_close(db);

    return 0;
}
