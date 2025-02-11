#include "utils/utils.hpp"
#include <iostream>
#include "ecs/aw_scene.hpp"

template <typename T> void enterType(T t) { std::cout << "Type: " << Airwave::demangle(typeid(t).name()) << std::endl; }
class Test
{
  public:
    Test(int v) : a(v) {}

    int a = 0;
};

int main()
{
    enterType<int>(1);
    enterType<float>(1.0f);
    enterType<Test>(Test(1));

    // Airwave::AwScene scene(nullptr);
    
    // auto scene = std::make_shared<Airwave::AwScene>();

    // auto entity = scene->createDefaultEntity("TestEntity", "TestTag");
    // scene->addComponent<Test>(entity, 1);

    return 0;
}