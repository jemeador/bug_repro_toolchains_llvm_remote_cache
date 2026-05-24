#include <format>
#include <iostream>

#include "flatbuffers/flatbuffers.h"
#include "message_generated.h"

int main() {
  flatbuffers::FlatBufferBuilder builder(256);

  auto text = builder.CreateString("Hello, FlatBuffers with C++20!");
  auto msg = Example::CreateMessage(builder, text, 42);
  builder.Finish(msg);

  auto buf = builder.GetBufferPointer();
  auto message = Example::GetMessage(buf);

  std::cout << std::format("Text: {}\n", message->text()->c_str());
  std::cout << std::format("ID: {}\n", message->id());

  return 0;
}
