#ifndef MENULIB_H
#define MENULIB_H

#define MAXMENUITEMS 20
#define NUMOFELEMS(N) (sizeof(N)/sizeof(N[0]))

#include <Arduino.h>

struct MenuItem
{
    const char *name; // name of item
    void *context;
    void (*action)(void *);
    Menu *rootMenu = nullptr;

    MenuItem()
        : name(nullptr), action(nullptr), context(nullptr) {}
    MenuItem(const char *n, void (*a)(void *), void *ctx)
        : name(n), action(a), context(ctx) {}
    MenuItem(const char *n, void (*a)(void *), Menu *root)
        : name(n), action(a), rootMenu(root) {}

    void executeAction();
};

class Menu
{
private:
    const char *title;
    MenuItem *items;
    size_t itemCount;

public:
    Menu(const char *t) : title(t), itemCount(0) {}

    Menu(const char *t, MenuItem *x, int numerOfItems) : title(t), itemCount(numerOfItems), items(x) {}

    void addItem(const char *name, void (*action)(void *), void *context);
    void executeItem(size_t index);
    String getNameItem(size_t index);
    String getMenuName();
};

#endif // MENULIB_H