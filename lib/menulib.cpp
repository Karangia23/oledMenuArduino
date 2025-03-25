#include "menulib.h"

void MenuItem::executeAction(){
    if(action != nullptr && rootMenu == false){
        action(context);
    }
    else{
        currentMenu = rootMenu;
    }
}