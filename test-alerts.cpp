#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include "test/catch.hpp"
#include "typewise-alert.h"

TEST_CASE("infers the breach according to limits") {
  REQUIRE(inferBreach(12, 20, 30) == TOO_LOW);
  REQUIRE(inferBreach(32, 20, 30) == TOO_HIGH);
  REQUIRE(inferBreach(22, 20, 30) == NORMAL);
}

TEST_CASE("classify temperate breach") {
  REQUIRE(classifyTemperatureBreach(PASSIVE_COOLING, 25) == NORMAL);
  REQUIRE(classifyTemperatureBreach(PASSIVE_COOLING, 36) == TOO_HIGH);
  REQUIRE(classifyTemperatureBreach(HI_ACTIVE_COOLING, 41) == NORMAL);
  REQUIRE(classifyTemperatureBreach(HI_ACTIVE_COOLING, 46) == TOO_HIGH);
  REQUIRE(classifyTemperatureBreach(MED_ACTIVE_COOLING, 36) == NORMAL);
  REQUIRE(classifyTemperatureBreach(MED_ACTIVE_COOLING, 41) == TOO_HIGH);
}

TEST_CASE("Test checkAndAlert(to email too high)") {

  BatteryCharacter batterych_HAC = {
    .coolingType = HI_ACTIVE_COOLING
  };

  std::ostringstream toEmail;
  std::streambuf* streambuf_1 = std::cout.rdbuf();
  std::cout.rdbuf(toEmail.rdbuf());

  checkAndAlert(TO_EMAIL, batterych_HAC, 46);
  
  //restore out stream
  std::cout.rdbuf(streambuf_1);
  REQUIRE(toEmail.str() == "To: a.b@c.com\nHi, the temperature is too high\n");
}

TEST_CASE("Test checkAndAlert(to email too low)") {
  
  BatteryCharacter batterych_PC = {
    .coolingType = PASSIVE_COOLING
  };
  
  std::ostringstream toEmail;
  std::streambuf* streambuf_2 = std::cout.rdbuf();
  std::cout.rdbuf(toEmail.rdbuf());

  checkAndAlert(TO_EMAIL, batterych_PC, -10);
  
  //restore out stream
  std::cout.rdbuf(streambuf_2);
  REQUIRE(toEmail.str() == "To: a.b@c.com\nHi, the temperature is too low\n");
}

TEST_CASE("Test checkAndAlert(to controller)") {

  BatteryCharacter batterych = {
    .coolingType = HI_ACTIVE_COOLING
  };

  std::ostringstream toController;
  std::streambuf* streambuf_1 = std::cout.rdbuf();
  std::cout.rdbuf(toController.rdbuf());

  checkAndAlert(TO_CONTROLLER, batterych, 50);
  
  //restore out stream
  std::cout.rdbuf(streambuf_1);
  REQUIRE(toController.str() == "65261 : 2\n");

}
