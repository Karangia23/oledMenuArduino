#define MAXMENUITEMS 20
#define NUMOFELEMS(N) (sizeof(N)/sizeof(N[0]))

#include <Arduino.h>

#ifdef testing
#include <functional>
#endif

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <string>

/*
    This enum show what is currently happening:
    - changeSliderValue is the default action, we are changing the value of some var,
    - robotRunning happens when the robot is running and should not be interrupted,
    - changeSliderValue, happens when we are changing the value via nice looking slider
*/

enum ActionMode
{
    changeSliderValue,
    robotRunning,
    changingValue
};

ActionMode currentMenuAction = changingValue;

Menu *currentMenu = nullptr;
int currentItemSelect = 0;

struct MenuItem {
    const char* name;               // name of item
    void *context;                  //argument of the function to execute
    void (*action)(void*);          //function pointer whcih is going to happen after clicking the menu postion 
    Menu *rootMenu = nullptr;       // pointer to the parent menu 
     

    MenuItem()
        : name(nullptr), action(nullptr), context(nullptr) {}
    MenuItem(const char* n, void (*a)(void*), void* ctx)
        : name(n), action(a), context(ctx) {}
    MenuItem(const char* n, void (*a)(void*), Menu* root)
        : name(n), action(a), rootMenu(root) {}

    void executeAction(){
        //
        if(action != nullptr && rootMenu == false){
            action(context);
        }
        //going up
        else{
            currentMenu = rootMenu;
        }
    }

};



class Menu {
private:
    const char* title;              
    MenuItem* items;           
    size_t itemCount = 0;     
    size_t currentIndex = 0;                                 
public:
    Menu(const char* t) : title(t), itemCount(0) {}

    Menu(const char* t, MenuItem* x, int numberOfItems) : title(t), itemCount(numberOfItems),items(x) {}

    void addItem(const char* name, void (*action)(void*), void* context ){
        if (itemCount < MAXMENUITEMS) { 
            items[itemCount] = MenuItem(name,action,context);
            itemCount++;
        }
    }
    void executeItem(size_t index) {
        if (index < itemCount && index >= 0) {
            items[index].executeAction();
        }
    }
    String getNameItem(size_t index) {
        return items[index].name;
    }
    String getMenuName(){
        return title;
    }

    int getNumberOfItems(){
        return itemCount;
    }
};
//-----------------------------------

extern Menu runMenu, optionsMenu, mainMenu;

MenuItem mainMenuItemList[] = {
    {"Start", nullptr, &runMenu},
    {"Options", nullptr, &optionsMenu},
    {"Go to chinese menu", changeMenu, (void*)&chineseMenu}
};

MenuItem runMenuItemList[] = {
    {"Test", nullptr, &runMenu},
    {"Go up", nullptr, &mainMenu},
};

MenuItem optionsMenuItemList[] = {
    {"Start", nullptr, &runMenu},
    {"Options", nullptr, &optionsMenu},
    {"Go to chinese menu", changeMenu, (void*)&chineseMenu}
};

Menu mainMenu = {
    "Main menu",
    mainMenuItemList,
    3};
    

Menu runMenu = {
    "Run menu"};

Menu optionsMenu = {
    "Options menu"};

Menu chineseMenu = {
    "I love china"};
//-----------------------------------

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define SCREEN_ADDRESS 0x3D
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(115200);


  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) { // Address 0x3D for 128x64, 0x78 on PCB
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  // Display static text
  display.println("Hello, world!");
  display.display(); 
}

void loop() {
    //renderMenu either every n seconds or newrender on isr (second more clever)
}


void renderMenu(Menu *currentMenu, int currentItem, int menuItems)
{
    if (menuItems < 1)
    {
        display.clearDisplay();
        display.setCursor(0, 14);

        display.println("No items in the menu :\(");
        
        display.display();
    }

    else if (menuItems < 2)
    {
        display.clearDisplay();
        display.setCursor(0, 14);

        display.println(currentMenu->getMenuName());

        display.print(currentItemSelect+1);
        display.print(currentMenu->getNameItem(currentItemSelect));
        display.println("<");
        
        display.display();
    }

    else if (menuItems < 3)
    {

        display.clearDisplay();
        display.setCursor(0, 14);

        display.println(currentMenu->getMenuName());


        if(currentItemSelect == 0){
        
            display.print(currentItemSelect+1);
            display.print(currentMenu->getNameItem(currentItemSelect));
            display.println("<");

            display.print(currentItemSelect+2);
            display.println(currentMenu->getNameItem(currentItemSelect+1));
        }

        else if (currentItemSelect == 1){
            display.print(currentItemSelect+1);
            display.println(currentMenu->getNameItem(currentItemSelect));
            

            display.print(currentItemSelect+2);
            display.print(currentMenu->getNameItem(currentItemSelect+1));
            display.println("<");
        }

        display.display();
    }

    else
    {
        display.clearDisplay();
        display.setCursor(0, 14);

        display.println(currentMenu->getMenuName());

        if (currentItem == 0)
        {
            // render 1,2,3 and select 1
            display.print(currentItemSelect+1);
            display.print(currentMenu->getNameItem(currentItemSelect));
            display.println("<");

            display.print(currentItemSelect+2);
            display.println(currentMenu->getNameItem(currentItemSelect+1));

            display.print(currentItemSelect+3);
            display.println(currentMenu->getNameItem(currentItemSelect+2));
        }
        else if (currentItem == menuItems - 1)
        {
            // render 1,2,3 from the last, and select 1 from the last
            display.print(currentItem-1);
            display.println(currentMenu->getNameItem(currentItem-2));

            display.print(currentItem);
            display.println(currentMenu->getNameItem(currentItem-1));

            display.print(currentItem+1);
            display.print(currentMenu->getNameItem(currentItem));
            display.println("<");
        }
        else
        {
            // render k-1, k and k+1 items, select k item
            display.print(currentItem);
            display.println(currentMenu->getNameItem(currentItem-1));

            display.print(currentItem+1);
            display.print(currentMenu->getNameItem(currentItem));
            display.println("<");

            display.print(currentItem+2);
            display.print(currentMenu->getNameItem(currentItem+1));
        }

        display.display();
    }
}

//here are functions for the menuitems (TODO)

void renderSliderOption(const char* sliderName, int changedValue)
{

}

void changeMenu(void *newMenuIndex){
    currentMenu = newMenuIndex;
}





//---------------------------------------------

void handleISR()
{

    switch (currentMenuAction)
    {
    case changingValue:
        break;
    case changeSliderValue:
        break;

    case robotRunning:
        break;

    default:
        break;
    }
}

void forwardButtonISR(){
    switch (currentMenuAction)
    {
    case changingValue:
        break;
    case changeSliderValue:
        break;

    case robotRunning:
        break;

    default:
        if(currentItemSelect<currentMenu->getNumberOfItems())
            currentItemSelect++;
        break;
    }
}

void backwardButtonISR(){
    switch (currentMenuAction)
    {
    case changingValue:
        break;
    case changeSliderValue:
        break;

    case robotRunning:
        break;

    default:
        if(currentItemSelect>0)
            currentItemSelect--;
        break;
    }
}

void acceptButtonISR(){
    switch (currentMenuAction)
    {
    case changingValue:
        break;
    case changeSliderValue:
        break;

    case robotRunning:
        break;

    default:
        currentMenu->executeItem(currentItemSelect);
        break;
    }
}