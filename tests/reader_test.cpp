#include "io/file_reader.hpp"
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>

class TempFile {
private:
  std::string path_;

public:
  explicit TempFile(const std::string &content) {
    path_ = "temp_test_file_" + std::to_string(rand()) + ".txt";
    std::ofstream file(path_);
    file << content;
    file.close();
  }

  ~TempFile() { std::filesystem::remove(path_); }

  const std::string &path() const { return path_; }
};

TEST(FileReaderTest, ReadNonExistentFile) {
  FileReader reader("nonexistent_file_xyz_123.txt");
  auto result = reader.read();

  EXPECT_TRUE(result.is_err());
  EXPECT_FALSE(result.error().empty());
}

TEST(FileReaderTest, ReadEmptyFile) {
  TempFile temp("");
  FileReader reader(temp.path());
  auto result = reader.read();

  ASSERT_TRUE(result.is_ok());
  EXPECT_TRUE(result.value().empty());
}

TEST(FileReaderTest, ReadSingleLine) {
  TempFile temp("single line");
  FileReader reader(temp.path());
  auto result = reader.read();

  ASSERT_TRUE(result.is_ok());
  EXPECT_EQ(result.value().size(), 1);
  EXPECT_EQ(result.value()[0], "single line");
}

TEST(FileReaderTest, ReadMultipleLines) {
  TempFile temp("line1\nline2\nline3");
  FileReader reader(temp.path());
  auto result = reader.read();

  ASSERT_TRUE(result.is_ok());
  EXPECT_EQ(result.value().size(), 3);
  EXPECT_EQ(result.value()[0], "line1");
  EXPECT_EQ(result.value()[1], "line2");
  EXPECT_EQ(result.value()[2], "line3");
}

TEST(FileReaderTest, ReadWithWindowsLineEndings) {
  TempFile temp("line1\r\nline2\r\nline3");
  FileReader reader(temp.path());
  auto result = reader.read();

  ASSERT_TRUE(result.is_ok());
  EXPECT_EQ(result.value().size(), 3);
  EXPECT_EQ(result.value()[0], "line1");
  EXPECT_EQ(result.value()[1], "line2");
  EXPECT_EQ(result.value()[2], "line3");
}

TEST(FileReaderTest, ReadWithEmptyLines) {
  TempFile temp("line1\n\nline3");
  FileReader reader(temp.path());
  auto result = reader.read();

  ASSERT_TRUE(result.is_ok());
  EXPECT_EQ(result.value().size(), 3);
  EXPECT_EQ(result.value()[0], "line1");
  EXPECT_EQ(result.value()[1], "");
  EXPECT_EQ(result.value()[2], "line3");
}

TEST(FileReaderTest, ReadWithTrailingNewline) {
  TempFile temp("line1\nline2\n");
  FileReader reader(temp.path());
  auto result = reader.read();

  ASSERT_TRUE(result.is_ok());
  EXPECT_EQ(result.value().size(), 2);
  EXPECT_EQ(result.value()[0], "line1");
  EXPECT_EQ(result.value()[1], "line2");
}

TEST(FileReaderTest, ReadWithSpecialCharacters) {
  TempFile temp("client_name-123\ntable@#$\n09:30");
  FileReader reader(temp.path());
  auto result = reader.read();

  ASSERT_TRUE(result.is_ok());
  EXPECT_EQ(result.value().size(), 3);
  EXPECT_EQ(result.value()[0], "client_name-123");
  EXPECT_EQ(result.value()[1], "table@#$");
  EXPECT_EQ(result.value()[2], "09:30");
}
