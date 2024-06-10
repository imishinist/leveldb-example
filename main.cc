#include <cassert>
#include <cstdint>
#include <iostream>
#include <vector>
#include <leveldb/db.h>
#include <leveldb/options.h>
#include <leveldb/slice.h>
#include <leveldb/write_batch.h>

void encode_to_hex(const leveldb::Slice input, std::string& output) {
    output.clear();
    output.push_back('[');
    for (size_t i = 0; i < input.size(); i++) {
        if (i > 0) {
            output.push_back(' ');
        }

        const unsigned char b = static_cast<char>(input[i]);
        output.push_back("0123456789ABCDEF"[b >> 4]);
        output.push_back("0123456789ABCDEF"[b & 0x0F]);
    }
    output.push_back(']');
}

void write_varint32(std::vector<uint8_t>& result, uint32_t value) {
    while (value > 0x7F) {
        result.push_back(static_cast<uint8_t>((value & 0x7F) | 0x80));
        value >>= 7;
    }
    result.push_back(static_cast<uint8_t>(value));
}

void read_all(leveldb::DB* db) {
    leveldb::ReadOptions read_options;
    read_options.verify_checksums = true;
    leveldb::Iterator* iter = db->NewIterator(read_options);
    iter->SeekToFirst();
    while (iter->Valid()) {
        std::string key, value;
        encode_to_hex(iter->key(), key);
        encode_to_hex(iter->value(), value);
        std::cout << "key: " << key << ", value: " << value << "\n";
        iter->Next();
    }
    delete iter;
}

void writes(leveldb::DB* db) {
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
    leveldb::Status status = db->Write(write_options, &batch);
    assert(status.ok());
}

void show_db(leveldb::DB* db) {
    {
        for (int i = 0; i < 7; i++) {
            std::string input = "leveldb.num-files-at-level" + std::to_string(i);
            std::string output;
            bool result = db->GetProperty(input, &output);
            assert(result);
            std::cout << "leveldb.num-files-at-level" << i << ": " << output << "\n";
        }
    }

    {
        std::string output;
        bool result = db->GetProperty("leveldb.stats", &output);
        assert(result);
        std::cout << output << "\n";
    }

    {
        std::string output;
        bool result = db->GetProperty("leveldb.sstables", &output);
        assert(result);
        std::cout << output << "\n";
    }

    {
        std::string output;
        bool result = db->GetProperty("leveldb.approximate-memory-usage", &output);
        assert(result);
        std::cout << "leveldb.approximate-memory-usage: " << output << "\n";
    }
}

int main() {
    std::string path = "/tmp/leveldb";

    leveldb::DB *db;
    leveldb::Options options;
    options.create_if_missing = true;

    leveldb::Status status = leveldb::DB::Open(options, path, &db);
    assert(status.ok());

    // writes(db);
    // read_all(db);
    // show_db(db);

    // db->CompactRange(nullptr, nullptr);

    delete db;

    return 0;
}
