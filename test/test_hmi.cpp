#include "Controller.hpp"
#include "keypad_factory_interface.hpp"
#include <unity.h>

void setUp()
{
}

void tearDown()
{
}

void test_Controller()
{
    const auto fakeKeypad = board::getKeypad();
    Controller controller(fakeKeypad);

    // First no key is pressed
    const auto event_cand = controller.checkHmiInput();
    TEST_ASSERT_EQUAL(false, event_cand.has_value());
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_Controller);

    UNITY_END();
}
