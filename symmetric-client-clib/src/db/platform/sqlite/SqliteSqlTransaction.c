/**
 * Licensed to JumpMind Inc under one or more contributor
 * license agreements.  See the NOTICE file distributed
 * with this work for additional information regarding
 * copyright ownership.  JumpMind Inc licenses this file
 * to you under the GNU General Public License, version 3.0 (GPLv3)
 * (the "License"); you may not use this file except in compliance
 * with the License.
 *
 * You should have received a copy of the GNU General Public License,
 * version 3.0 (GPLv3) along with this library; if not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */
#include <db/sqlite/SqliteSqlTransaction.h>

int SymSqliteSqlTransaction_queryForInt(SymSqliteSqlTransaction *this, char *sql, SymStringArray *args, SymList *sqlTypes, int *error) {
    return this->sqlTemplate->queryForInt(this->sqlTemplate, sql, args, sqlTypes, error);
}

char * SymSqliteSqlTransaction_queryForString(SymSqliteSqlTransaction *this, char *sql, SymStringArray *args, SymList *sqlTypes, int *error) {
    return this->sqlTemplate->queryForString(this->sqlTemplate, sql, args, sqlTypes, error);
}

void SymSqliteSqlTransaction_query(SymSqliteSqlTransaction *this, char *sql, SymStringArray *args, SymList *sqlTypes, int *error, void *callback) {
    this->sqlTemplate->query(this->sqlTemplate, sql, args, sqlTypes, error, callback);
}

static void SymSqliteSqlTransaction_requireTransaction(SymSqliteSqlTransaction *this) {
    if (!this->inTransaction) {
        sqlite3_exec(this->db, "BEGIN TRANSACTION", NULL, NULL, NULL);
        this->inTransaction = 1;
    }
}

void SymSqliteSqlTransaction_prepare(SymSqliteSqlTransaction *this, char *sql) {
    this->sql = sql;
    SymSqliteSqlTransaction_requireTransaction(this);
    printf("Preparing %s\n", sql);
    int rc = sqlite3_prepare_v2(this->db, sql, -1, &this->stmt, NULL);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sql);
        fprintf(stderr, "SQL Exception: %s\n", sqlite3_errmsg(this->db));
    }
}

int SymSqliteSqlTransaction_addRow(SymSqliteSqlTransaction *this, SymStringArray *args, SymList *sqlTypes) {
    // TODO: do we need to convert to sqlType and bind correctly?

    sqlite3_reset(this->stmt);
    printf("Add Row [");
    int i;
    for (i = 0; args != NULL && i < args->size; i++) {
        char *arg = args->get(args, i);
        if (arg) {
            sqlite3_bind_text(this->stmt, i + 1, arg, -1, SQLITE_STATIC);
        } else {
            sqlite3_bind_null(this->stmt, i + 1);
        }

        if (i > 0) {
            printf(",");
        }
        printf("%s", arg);
    }
    printf("]\n");

    int rc = sqlite3_step(this->stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Failed to execute statement: %s\n", this->sql);
        fprintf(stderr, "SQL Exception: %s\n", sqlite3_errmsg(this->db));
    }
    return sqlite3_changes(this->db);
}

int SymSqliteSqlTransaction_update(SymSqliteSqlTransaction *this, char *sql, SymStringArray *args, SymList *sqlTypes, int *error) {
    SymSqliteSqlTransaction_requireTransaction(this);
    return this->sqlTemplate->update(this->sqlTemplate, sql, args, sqlTypes, error);
}

void SymSqliteSqlTransaction_commit(SymSqliteSqlTransaction *this) {
    sqlite3_exec(this->db, "COMMIT", NULL, NULL, NULL);
    this->inTransaction = 0;
}

void SymSqliteSqlTransaction_rollback(SymSqliteSqlTransaction *this) {
    sqlite3_exec(this->db, "ROLLBACK", NULL, NULL, NULL);
    this->inTransaction = 0;
}

void SymSqliteSqlTransaction_close(SymSqliteSqlTransaction *this) {
    if (this->stmt != NULL) {
        sqlite3_finalize(this->stmt);
    }
    this->stmt = NULL;
}

void SymSqliteSqlTransaction_destroy(SymSqliteSqlTransaction *this) {
    SymSqliteSqlTransaction_close(this);
    free(this);
}

SymSqliteSqlTransaction * SymSqliteSqlTransaction_new(SymSqliteSqlTransaction *this, SymSqliteSqlTemplate *sqlTemplate) {
    if (this == NULL) {
        this = (SymSqliteSqlTransaction *) calloc(1, sizeof(SymSqliteSqlTransaction));
    }
    this->sqlTemplate = (SymSqlTemplate *) sqlTemplate;
    this->db = sqlTemplate->db;
    SymSqlTransaction *super = (SymSqlTransaction *) this;
    super->queryForInt = (void *) &SymSqliteSqlTransaction_queryForInt;
    super->queryForString = (void *) &SymSqliteSqlTransaction_queryForString;
    super->query = (void *) &SymSqliteSqlTransaction_query;
    super->update = (void *) &SymSqliteSqlTransaction_update;
    super->prepare = (void *) &SymSqliteSqlTransaction_prepare;
    super->addRow = (void *) &SymSqliteSqlTransaction_addRow;
    super->commit = (void *) &SymSqliteSqlTransaction_commit;
    super->rollback = (void *) &SymSqliteSqlTransaction_rollback;
    super->close = (void *) &SymSqliteSqlTransaction_close;
    this->destroy = (void *) &SymSqliteSqlTransaction_destroy;
    return this;
}
