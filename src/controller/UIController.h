#include "../ui/IUIScreen.h"
#include "../ui/intro/introScreen.h"

class UIController {
    private:
        IUIScreen* currentScreen;
        Arduino_RGB_Display* gfx;
        
        IntroScreen* introScreen;

    public:
        UIController();
        ~UIController();

        void begin();
        void drawCurrentScreen();
        uint8_t handleTouch(int, int);
};
