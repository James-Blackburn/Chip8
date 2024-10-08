#include <gtest/gtest.h>

#include "display.hpp"

TEST(DisplayTest, DisplayInitialisation) {
    Display display;
    ASSERT_NO_THROW(display.init()) << "Failed to initialise display.";
}

TEST(DisplayTest, ManipulatingPixels) { 
    Display display;
    
    // write all pixels
    unsigned char ticker = 0;
    for (int y = 0; y < DISPLAY_HEIGHT; y++) { 
        for (int x = 0; x < DISPLAY_WIDTH; x++) { 
            ASSERT_NO_THROW(display.setPixel(x, y, ticker % 255));
            ticker++;
        }
    }

    // Validate correct pixels have been written
    ticker = 0;
    for (int y = 0; y < DISPLAY_HEIGHT; y++) { 
        for (int x = 0; x < DISPLAY_WIDTH; x++) { 
            ASSERT_EQ(display.readPixel(x, y), ticker % 255);
            ticker++;
        }
    }
}

TEST(DisplayTest, ClearingPixels) { 
    Display display;

    // Fill display with test pixels
    for (int y = 0; y < DISPLAY_HEIGHT; y++) { 
        for (int x = 0; x < DISPLAY_WIDTH; x++) { 
            display.setPixel(x, y, 1);
        }
    }

    // Clear and verify pixels are 0
    display.clear();
    for (int y = 0; y < DISPLAY_HEIGHT; y++) { 
        for (int x = 0; x < DISPLAY_WIDTH; x++) { 
            ASSERT_EQ(display.readPixel(x, y), 0);
        }
    }
}

TEST(DisplayTest, HandlesInvalidPixels) { 
    Display display;

    // Check setPixel
    ASSERT_THROW(display.setPixel(-1, -1, 1), std::out_of_range);
    ASSERT_THROW(display.setPixel(-1, DISPLAY_HEIGHT, 1), std::out_of_range);
    ASSERT_THROW(display.setPixel(DISPLAY_WIDTH, DISPLAY_HEIGHT, 1), std::out_of_range);
    ASSERT_THROW(display.setPixel(DISPLAY_WIDTH, -1, 1), std::out_of_range);

    // Check readPixel
    ASSERT_THROW(display.readPixel(-1, -1), std::out_of_range);
    ASSERT_THROW(display.readPixel(-1, DISPLAY_HEIGHT), std::out_of_range);
    ASSERT_THROW(display.readPixel(DISPLAY_WIDTH, DISPLAY_HEIGHT), std::out_of_range);
    ASSERT_THROW(display.readPixel(DISPLAY_WIDTH, -1), std::out_of_range);
}