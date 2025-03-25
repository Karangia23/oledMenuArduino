#include <unity.h>

bool test_menu_constructor(){

    return false;
}

int main(int argc, char** argv){
    UNITY_BEGIN();
    TEST_ASSERT_TRUE(test_menu_constructor);
    UNITY_END();
}