#include <cassert>
#include <cstdint>
#include <vector>
#include <leveldb/db.h>
#include <leveldb/options.h>
#include <leveldb/write_batch.h>

void write_varint32(std::vector<uint8_t>& result, uint32_t value) {
    while (value > 0x7F) {
        result.push_back(static_cast<uint8_t>((value & 0x7F) | 0x80));
        value >>= 7;
    }
    result.push_back(static_cast<uint8_t>(value));
}

int main() {
    std::string path = "/tmp/leveldb";

    leveldb::DB *db;
    leveldb::Options options;
    options.create_if_missing = true;

    leveldb::Status status = leveldb::DB::Open(options, path, &db);
    assert(status.ok());

    leveldb::WriteBatch batch;
    for (int i = 0; i < 1024; i++) {
        std::vector<uint8_t> input;
        write_varint32(input, i);
        std::string key(input.begin(), input.end());
        std::string value = std::to_string(i);

        batch.Put(key, value);
    }

    leveldb::WriteOptions write_options;
    write_options.sync = true;
    status = db->Write(write_options, &batch);
    assert(status.ok());

    delete db;

    return 0;
}
