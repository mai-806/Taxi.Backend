#include "hello.hpp"

#include <userver/utest/utest.hpp>

UTEST(SayHelloTo, Basic) {
  EXPECT_EQ(APIGateway::SayHelloTo("Developer"), "Hello, Developer!\n");
  EXPECT_EQ(APIGateway::SayHelloTo({}), "Hello, unknown user!\n");
}
