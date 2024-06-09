#include <cassert>
#include <leveldb/db.h>
#include <leveldb/options.h>

int main() {
    std::string path = "/tmp/leveldb";

    leveldb::DB *db;
    leveldb::Options options;
    options.create_if_missing = true;

    leveldb::Status status = leveldb::DB::Open(options, path, &db);
    assert(status.ok());

    std::string key = "name";
    std::string value = "LevelDB";
    status = db->Put(leveldb::WriteOptions(), key, value);
    assert(status.ok());

    delete db;

    return 0;
}
