#include <Arduino.h>
#include <TFT_eSPI.h>
#include <lvgl.h>
#include "BleCombo.h" // New custom NimBLE class (Keyboard Only)
#include <Wire.h>
#include "Touch.h"
#include "esp_system.h" 

// Screen resolution
static const uint16_t screenWidth  = 480;
static const uint16_t screenHeight = 320;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[screenWidth * 5]; // Reduced buffer to save RAM

TFT_eSPI tft = TFT_eSPI();
BleCombo bleCombo("GSPRO Controller");
Touch touch;

// Forward Declaration
void load_main_ui();

/* Debug & Status globals */
static lv_obj_t * volatile g_status_label = NULL;
static lv_timer_t * g_status_timer = NULL; // Global timer handle

/* Key Mapping Struct */
struct KeyMap {
    const char *label;
    uint8_t key;
    uint8_t modifier; 
    bool repeat;
};

/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);
    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors((uint16_t *)&color_p->full, w * h, true);
    tft.endWrite();
    lv_disp_flush_ready(disp);
}

/* Touch Reading */
void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data) {
    uint16_t touchX, touchY;
    bool touched = touch.getTouch(&touchX, &touchY);

    if (!touched) {
        data->state = LV_INDEV_STATE_REL;
    } else {
        data->state = LV_INDEV_STATE_PR;
        
        // Map raw (Portrait) to Landscape
        data->point.x = touchY;
        data->point.y = 320 - touchX;
        
        // Serial.printf("Touch: (%d, %d) Raw: (%d, %d)\n", data->point.x, data->point.y, touchX, touchY);

        if(data->point.x < 0) data->point.x = 0;
        if(data->point.x >= screenWidth) data->point.x = screenWidth - 1;
        if(data->point.y < 0) data->point.y = 0;
        if(data->point.y >= screenHeight) data->point.y = screenHeight - 1;
    }
}

/* I2C Scanner for Debug */
void scanI2C() {
    byte error, address;
    int nDevices = 0;
    Serial.println("Scanning I2C...");
    for(address = 1; address < 127; address++ ) {
        Wire.beginTransmission(address);
        error = Wire.endTransmission();
        if (error == 0) {
            Serial.printf("I2C device found at address 0x%02X\n", address);
            nDevices++;
        }
    }
    if (nDevices == 0) Serial.println("No I2C devices found\n");
}

/* UI Styles */
static lv_style_t style_scr;
static lv_style_t style_btn_action;
static lv_style_t style_btn_nav;
static lv_style_t style_title;

void init_styles() {
    // Screen Style
    lv_style_init(&style_scr);
    lv_style_set_bg_color(&style_scr, lv_color_hex(0x121212)); // Dark Grey
    lv_style_set_text_color(&style_scr, lv_color_white());

    // Action Button Style (High Contrast)
    lv_style_init(&style_btn_action);
    lv_style_set_radius(&style_btn_action, 12);
    lv_style_set_bg_color(&style_btn_action, lv_color_hex(0x333333)); // Lighter Charcoal for visibility
    lv_style_set_bg_grad_color(&style_btn_action, lv_color_hex(0x1a1a1a)); // Darker bottom
    lv_style_set_bg_grad_dir(&style_btn_action, LV_GRAD_DIR_VER);
    lv_style_set_border_color(&style_btn_action, lv_color_hex(0x00E676)); // Bright Neon Green Border
    lv_style_set_border_width(&style_btn_action, 2);
    lv_style_set_shadow_width(&style_btn_action, 15);
    lv_style_set_shadow_color(&style_btn_action, lv_color_hex(0x00E676)); // Green Glow
    lv_style_set_shadow_opa(&style_btn_action, LV_OPA_20);
    lv_style_set_text_color(&style_btn_action, lv_color_white()); // FORCE WHITE TEXT
    lv_style_set_text_font(&style_btn_action, &lv_font_montserrat_14);

    // Nav Button Style (Blue Accent)
    lv_style_init(&style_btn_nav);
    lv_style_set_radius(&style_btn_nav, 50); // Circular/Pill
    lv_style_set_bg_color(&style_btn_nav, lv_color_hex(0x2196F3)); // Brighter Blue
    lv_style_set_bg_grad_color(&style_btn_nav, lv_color_hex(0x0D47A1));
    lv_style_set_bg_grad_dir(&style_btn_nav, LV_GRAD_DIR_VER);
    lv_style_set_shadow_width(&style_btn_nav, 10);
    lv_style_set_shadow_color(&style_btn_nav, lv_color_hex(0x000000));
    lv_style_set_text_color(&style_btn_nav, lv_color_white()); // FORCE WHITE TEXT
    lv_style_set_text_font(&style_btn_nav, &lv_font_montserrat_14);
    lv_style_set_border_width(&style_btn_nav, 1);
    lv_style_set_border_color(&style_btn_nav, lv_color_hex(0x64B5F6)); // Lighter Blue Border

    // Title Style
    lv_style_init(&style_title);
    lv_style_set_text_font(&style_title, &lv_font_montserrat_14);
    lv_style_set_text_color(&style_title, lv_color_hex(0xFFFFFF));
}

/* Event Handler */
void btn_event_handler(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    KeyMap *k = (KeyMap *)lv_event_get_user_data(e);
    
    if (!bleCombo.isConnected()) return;

    if (k->repeat) {
        if (code == LV_EVENT_PRESSED) {
            bleCombo.k_press(k->key);
            if (g_status_label) lv_label_set_text_fmt(g_status_label, "Holding: %s", k->label);
        } else if (code == LV_EVENT_RELEASED || code == LV_EVENT_PRESS_LOST) {
            bleCombo.k_release(k->key);
            if (g_status_label) lv_label_set_text(g_status_label, "Released");
        }
    } else {
        if (code == LV_EVENT_CLICKED) {
            if (g_status_label) lv_label_set_text_fmt(g_status_label, "Sent: %s", k->label);

            if (k->modifier == 1) {
                bleCombo.k_press(KEY_LEFT_CTRL);
                bleCombo.k_press(k->key);
                delay(50);
                bleCombo.k_releaseAll();
            } else {
                bleCombo.k_write(k->key);
            }
        }
    }
}

/* Helper to Create Buttons */
lv_obj_t* create_custom_btn(lv_obj_t *parent, const char *symbol, const char *text, int x, int y, int w, int h, KeyMap *km, lv_style_t *style) {
    lv_obj_t *btn = lv_btn_create(parent);
    lv_obj_set_size(btn, w, h);
    lv_obj_align(btn, LV_ALIGN_TOP_LEFT, x, y);
    lv_obj_add_style(btn, style, 0);
    lv_obj_add_event_cb(btn, btn_event_handler, LV_EVENT_ALL, km);

    lv_obj_t *content = lv_obj_create(btn); // Container for text/icon
    lv_obj_set_size(content, LV_PCT(100), LV_PCT(100));
    lv_obj_set_style_bg_opa(content, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(content, 0, 0);
    lv_obj_set_flex_flow(content, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(content, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_center(content);
    lv_obj_set_scroll_dir(content, LV_DIR_NONE);
    lv_obj_clear_flag(content, LV_OBJ_FLAG_CLICKABLE); // IMPORTANT: Let clicks pass through to the button

    if(symbol) {
        lv_obj_t *lbl_sym = lv_label_create(content);
        lv_label_set_text(lbl_sym, symbol);
    }
    
    if(text) {
        lv_obj_t *lbl_txt = lv_label_create(content);
        lv_label_set_text(lbl_txt, text);
        lv_obj_set_style_text_align(lbl_txt, LV_TEXT_ALIGN_CENTER, 0); 
        lv_obj_set_style_text_color(lbl_txt, lv_color_white(), 0); // Force White Text on Label
    }

    return btn;
}

void load_main_ui() {
    lv_obj_t *scr = lv_scr_act();
    lv_obj_clean(scr); // Clear splash
    lv_obj_add_style(scr, &style_scr, 0);

    // Header Bar
    lv_obj_t *header = lv_obj_create(scr);
    lv_obj_set_size(header, 480, 40);
    lv_obj_align(header, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_bg_color(header, lv_color_hex(0x1F1F1F), 0);
    lv_obj_set_style_border_side(header, LV_BORDER_SIDE_BOTTOM, 0);
    lv_obj_set_style_border_width(header, 2, 0);
    lv_obj_set_style_border_color(header, lv_color_hex(0x4CAF50), 0);
    lv_obj_set_scrollbar_mode(header, LV_SCROLLBAR_MODE_OFF);

    // Logo Text in Header
    lv_obj_t *logo_gs = lv_label_create(header);
    lv_label_set_text(logo_gs, "#4CAF50 GS# #FFFFFF PRO#");
    lv_label_set_recolor(logo_gs, true);
    lv_obj_align(logo_gs, LV_ALIGN_LEFT_MID, 10, 0);

    // Status Label in Header
    g_status_label = lv_label_create(header);
    lv_label_set_text(g_status_label, "Waiting for BT...");
    lv_obj_align(g_status_label, LV_ALIGN_CENTER, 0, 0);

    // Timer to update header status
    if (g_status_timer) lv_timer_del(g_status_timer); // Clean up old timer
    g_status_timer = lv_timer_create([](lv_timer_t * t) {
        bool connected = bleCombo.isConnected();
        
        // Update Status Label
        if (g_status_label && lv_obj_is_valid(g_status_label)) {
             if(connected) {
                lv_label_set_text(g_status_label, "BT Connected");
                lv_obj_set_style_text_color(g_status_label, lv_color_hex(0x4CAF50), 0);
            } else {
                lv_label_set_text(g_status_label, "BT Disconnected");
                lv_obj_set_style_text_color(g_status_label, lv_color_hex(0xF44336), 0);
            }
        }
    }, 1000, NULL);


    // --- LEFT COLUMN: Game Actions ---
    static KeyMap kmMulligan = {"Mulligan", 'm', 1, false}; // Ctrl+M
    create_custom_btn(scr, NULL, "Mulligan", 20, 60, 100, 70, &kmMulligan, &style_btn_action);

    static KeyMap kmPin = {"Pin", 'p', 0, false};
    create_custom_btn(scr, NULL, "Pin", 20, 145, 100, 70, &kmPin, &style_btn_action);

    static KeyMap kmScout = {"Scout", 'j', 0, false};
    create_custom_btn(scr, NULL, "Scout", 20, 230, 100, 70, &kmScout, &style_btn_action);

    // --- MIDDLE COLUMN: Views & Tees ---
    static KeyMap kmHeat = {"Heat", 'z', 0, false}; // 'z' -> 'y' on German Layout
    create_custom_btn(scr, NULL, "Heat", 140, 60, 100, 70, &kmHeat, &style_btn_action);

    static KeyMap kmFly = {"Fly", 'o', 0, false};
    create_custom_btn(scr, NULL, "Flyover", 140, 145, 100, 70, &kmFly, &style_btn_action);

    static KeyMap kmFree = {"Free", KEY_F5, 0, false};
    create_custom_btn(scr, NULL, "Free", 140, 230, 100, 70, &kmFree, &style_btn_action);


    // --- RIGHT AREA: Aim & Tees ---
    // Group Background for Controls
    lv_obj_t *ctrl_bg = lv_obj_create(scr);
    lv_obj_set_size(ctrl_bg, 220, 240);
    lv_obj_align(ctrl_bg, LV_ALIGN_TOP_RIGHT, -10, 60);
    lv_obj_set_style_bg_color(ctrl_bg, lv_color_hex(0x222222), 0);
    lv_obj_set_style_radius(ctrl_bg, 16, 0);
    lv_obj_set_style_border_width(ctrl_bg, 0, 0);
    
    // Arrow Keys (Nav Style) - Text Only - SHORTENED
    static KeyMap kmUp = {"Up", KEY_UP_ARROW, 0, true};
    create_custom_btn(scr, NULL, "UP", 335, 75, 60, 60, &kmUp, &style_btn_nav);

    static KeyMap kmLeft = {"Left", KEY_LEFT_ARROW, 0, true};
    create_custom_btn(scr, NULL, "LT", 265, 145, 60, 60, &kmLeft, &style_btn_nav);

    static KeyMap kmRight = {"Right", KEY_RIGHT_ARROW, 0, true};
    create_custom_btn(scr, NULL, "RT", 405, 145, 60, 60, &kmRight, &style_btn_nav);

    static KeyMap kmDown = {"Down", KEY_DOWN_ARROW, 0, true};
    create_custom_btn(scr, NULL, "DN", 335, 145, 60, 60, &kmDown, &style_btn_nav);

    // Tee Buttons
    static KeyMap kmTeeL = {"Tee L", 'c', 0, true};
    create_custom_btn(scr, NULL, "Tee L", 265, 225, 95, 50, &kmTeeL, &style_btn_nav);

    static KeyMap kmTeeR = {"Tee R", 'v', 0, true}; // Init struct correctly
    create_custom_btn(scr, NULL, "Tee R", 370, 225, 95, 50, &kmTeeR, &style_btn_nav);
}

void show_splash_screen() {
    lv_obj_t *scr = lv_scr_act();
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x000000), 0);

    // Big Logo
    lv_obj_t *logo = lv_label_create(scr);
    lv_label_set_text(logo, "#4CAF50 GS# #FFFFFF PRO#");
    lv_label_set_recolor(logo, true);
    // Scale text manually if needed or use a larger font if available. 
    // Standard font is small, so we use zoom to make it huge
    lv_obj_set_style_text_font(logo, &lv_font_montserrat_14, 0); 
    lv_obj_set_style_transform_zoom(logo, 512, 0); // 2x scale
    lv_obj_center(logo);

    lv_obj_t *sub = lv_label_create(scr);
    lv_label_set_text(sub, "Controller Loading...");
    lv_obj_set_style_text_color(sub, lv_color_hex(0x888888), 0);
    lv_obj_align(sub, LV_ALIGN_BOTTOM_MID, 0, -40);

    // One-shot timer to transition
    lv_timer_create([](lv_timer_t *t){
        load_main_ui();
        lv_timer_del(t);
    }, 3000, NULL);
}

void setup() {
    Serial.begin(115200);
    delay(500);

    // --- FORCE NEW MAC ADDRESS TO FIX WINDOWS CACHING ISSUES ---
    uint8_t new_mac[6] = {0xA4, 0xE5, 0x7C, 0xFA, 0x71, 0xDD}; // Set last byte to 0xDD
    esp_base_mac_addr_set(new_mac);
    Serial.printf("New MAC Address Set: %02X:%02X:%02X:%02X:%02X:%02X\n", 
                  new_mac[0], new_mac[1], new_mac[2], new_mac[3], new_mac[4], new_mac[5]);
    
    Wire.begin(18, 19);
    touch.begin(); // Increased speed inside Touch.cpp

    tft.begin();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    
    pinMode(23, OUTPUT);
    digitalWrite(23, HIGH);

    bleCombo.begin();
    scanI2C();

    lv_init();
    lv_disp_draw_buf_init(&draw_buf, buf, NULL, screenWidth * 5);

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register(&indev_drv);

    init_styles();
    show_splash_screen();
    
    Serial.println("System Ready");
}

void loop() {
    lv_timer_handler();
    delay(5);
}
