﻿
#include "Joystick.h"

#define JOYCON_BUTTON_COUNT 15

#define JOYCON_L_PRODUCT_ID 8198
#define JOYCON_R_PRODUCT_ID 8199
#define DUALSHOCK4_PRODUCT_ID 0x0
#define XBOX360_PRODUCT_ID 0x0

namespace altseed {

    bool Joystick::Initialize(JoystickType type)
    {
        types.fill(JoystickType::Other);

        this->RefreshConnectedState();
        globalCount = 0;
        
        for (int32_t i = 0; i < MAX_JOYSTICKS_NUM; i++)
        {
            currentHit[i].fill(false);
            preHit[i].fill(false);
            vibrateStates[i] = {};
//            currentAxis[i].fill(0);
        }
    
        return true;
    };
    
    void Joystick::Terminate() {
        for (int32_t i = 0; i < MAX_JOYSTICKS_NUM; i++)
        {
            hid_close(handler[i]);
        }
    };
    
    void Joystick::SendSubcommand(hid_device *dev, uint8_t command, uint8_t data[], int len) {
        uint8_t buf[0x40]; memset(buf, 0x0, size_t(0x40));
        
        buf[0] = 1; // 0x10 for rumble only
        buf[1] = globalCount; // Increment by 1 for each packet sent. It loops in 0x0 - 0xF range.
        
        if (globalCount == 0xf0) {
            globalCount = 0x00;
        } else {
            globalCount++;
        }
        
        buf[10] = command;
        memcpy(buf + 11, data, len);
        
        hid_write(dev, buf, 0x40);
    }
    
    void Joystick::RefreshConnectedState()
    {
//        hidハンドラーの取得
        hid_device_info *device = hid_enumerate(0, 0);
        const char *path;
        int i = 0;
        
        while (device)
        {
            path = device->path;
            
            if ( device->product_id == JOYCON_L_PRODUCT_ID || device->product_id == JOYCON_R_PRODUCT_ID
                || device->product_id == DUALSHOCK4_PRODUCT_ID || device->product_id == XBOX360_PRODUCT_ID )
            {
                hid_device *dev = hid_open(device->vendor_id, device->product_id, device->serial_number);
                
                hid_set_nonblocking(dev, 1);
                handler[i] = dev;
                names[i] = device->product_string;
                types[i] = (JoystickType)device->product_id;
                
                if(types[i] == JoystickType::JoyconL || types[i] == JoystickType::JoyconR)
                {
                    //        enable vibration for joycon
                    uint8_t data[0x01];
                    data[0] = 0x1;
                    SendSubcommand(dev, 0x48, data, 1);
                    
                    data[0] = 0x3F;
                    SendSubcommand(dev, 0x03, data, 1);
                }
                i++;
            }
            device = device->next;
        }
        hid_free_enumeration(device);
    };
    
    
//    NOTE: pushing in stick is handled as button
    void Joystick::HandleJoyconInput(int index, unsigned char *buff)
    {
        if (*buff == 0x21)
        {
            //buttons
            for (int i = 0; i <= 8; i++)
            {
                currentHit[index][i] = buff[4] & (1 << i);
            }
        }
        else if (*buff == 0x3F)
        {
//            hundle buttons
            for (int i = 0; i <= 8; i++)
            {
                currentHit[index][i] = buff[1] & (1 << i);
                currentHit[index][i + 8] = buff[2] & (1 << i);
            }
            
            switch (buff[3]) {
//                    Top
                case 0:
                    currentAxis[index][(int32_t)JoystickAxisType::LeftH] = 0;
                    currentAxis[index][(int32_t)JoystickAxisType::LeftV] = 1;
                    break;
//                    Top-right
                case 1:
                    currentAxis[index][(int32_t)JoystickAxisType::LeftH] = 1;
                    currentAxis[index][(int32_t)JoystickAxisType::LeftV] = 1;
                    break;
//                    Right
                case 2:
                    currentAxis[index][(int32_t)JoystickAxisType::LeftH] = 1;
                    currentAxis[index][(int32_t)JoystickAxisType::LeftV] = 0;
                    break;
//                    Bottom-right
                case 3:
                    currentAxis[index][(int32_t)JoystickAxisType::LeftH] = 1;
                    currentAxis[index][(int32_t)JoystickAxisType::LeftV] = -1;
                    break;
//                    Bottom
                case 4:
                    currentAxis[index][(int32_t)JoystickAxisType::LeftH] = 0;
                    currentAxis[index][(int32_t)JoystickAxisType::LeftV] = -1;
                    break;
//                    Bottom-Left
                case 5:
                    currentAxis[index][(int32_t)JoystickAxisType::LeftH] = -1;
                    currentAxis[index][(int32_t)JoystickAxisType::LeftV] = -1;
                    break;
//                    Left
                case 6:
                    currentAxis[index][(int32_t)JoystickAxisType::LeftH] = -1;
                    currentAxis[index][(int32_t)JoystickAxisType::LeftV] = 0;
                    break;
//                    Top-left
                case 7:
                    currentAxis[index][(int32_t)JoystickAxisType::LeftH] = -1;
                    currentAxis[index][(int32_t)JoystickAxisType::LeftV] = 1;
                    break;
                    
                default:
                    currentAxis[index][(int32_t)JoystickAxisType::LeftH] = 0;
                    currentAxis[index][(int32_t)JoystickAxisType::LeftV] = 0;
                    break;
            }
        }
    };
    
    void Joystick::RefreshInputState() {
        
        preHit = currentHit;

        for (int jind = 0; jind < MAX_JOYSTICKS_NUM; jind++)
        {
            if(!handler[jind])
                return;
            
//            read input report
            uint8_t buff[0x40]; memset(buff, 0x40, size_t(0x40));
            size_t size = 49;

            int ret = hid_read(handler[jind], buff, size);
//
            if (!ret) {
                return;
            }
            
            if (types[jind] == JoystickType::JoyconL || types[jind] == JoystickType::JoyconR)
            {
                HandleJoyconInput(jind, buff);
            }
        }
    };
    
    void Joystick::RefreshVibrateState()
    {
//        set output report
        uint8_t buf[0x40]; memset(buf, 0x0, size_t(0x40));
        buf[0] = 0x01; // 0x10 for rumble only
        buf[1] = 0x01; // Increment by 1 for each packet sent. It loops in 0x0 - 0xF range.
        
//        set vibration data
        uint8_t rumble_data[0x08]; memset(rumble_data, 0, size_t(0x08));
        
        std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
        
        for (int jind = 0; jind < MAX_JOYSTICKS_NUM; jind++)
        {
            if (vibrateStates[jind].life_time <= 0)
//                printf("skipped");
                continue;
            
            if (std::chrono::duration_cast<std::chrono::milliseconds>(now - vibrateStates[jind].pre_time).count()
                >= vibrateStates[jind].life_time)
            {
                vibrateStates[jind].life_time = 0;
                rumble_data[0] = 0x0;
                rumble_data[1] = 0x1;
                rumble_data[2] = 0x40;
                rumble_data[3] = 0x40;
                
                for (int i =4; i <= 7; i++) {
                    rumble_data[i] = rumble_data[i-4];
                }
                
                memcpy(buf + 2, rumble_data, 8);
                
                hid_write(handler[jind], buf, 0x40);
                
                vibrateStates[jind].isSended = false;
            }
            else if (!vibrateStates[jind].isSended)
            {
                float hf = std::clamp(vibrateStates[jind].high_frequency, 81.75177f, 1252.572266f);
                float lf = std::clamp(vibrateStates[jind].low_frequency, 40.875885f, 626.286133f);

                uint16_t high_f = ((uint8_t)round(log2((double)hf /10.0)*32.0) - 0x60) * 4;
                uint8_t high_amp = std::clamp(vibrateStates[jind].high_amplitude, 0.0f, 1.0f);
                
                uint8_t low_f = (uint8_t)round(log2((double)lf / 10.0 ) * 32.0) - 0x40;
                uint8_t low_amp = std::clamp(vibrateStates[jind].low_amplitude, 0.0f, 1.0f);
                
                uint8_t h_a = 0x00;
                if (high_amp < 0.117) {
                    h_a = ((log2(high_amp * 1000) * 32) - 0x60) / (5 - std::pow(2, high_amp)) - 1;
                }
                else if (high_amp >= 0.117 && high_amp < 0.23) {
                     h_a = ((log2(high_amp * 1000) * 32) - 0x60) - 0x5c;
                }
                else if (high_amp >= 0.23) {
                     h_a = (((log2(high_amp * 1000) * 32) - 0x60) * 2) - 0xf6;
                }
                
                uint8_t encoded = round(h_a) / 2;
                uint8_t evenOrOdd = encoded % 2;

                if (evenOrOdd > 0)
                  encoded = encoded - 1;

                encoded = encoded >> 1;

                uint8_t l_a = encoded + 0x40;


                rumble_data[0] = high_f & 0xFF;
                rumble_data[1] = h_a + ((high_f >> 8) & 0xFF);

                rumble_data[2] = low_f + ((l_a >> 8) & 0xFF);
                rumble_data[3] = l_a & 0xFF;
                
                for (int i =4; i <= 7; i++) {
                    rumble_data[i] = rumble_data[i-4];
                }
                
                memcpy(buf + 2, rumble_data, 8);
                
                hid_write(handler[jind], buf, 0x31);
                vibrateStates[jind].isSended = true;
                
                for (int i=0; i <= 3; i++) {
                    printf("\n%d: %x", i,rumble_data[i]);
                }
            }
        }
    };

//    life-time is milliseconds.
//    ex. SetVibration(1, 500.0f, 100.0f, 0.9f, 0.9f, 200)
    void Joystick::SetVibration(int16_t index, float high_freq, float low_freq, float high_amp, float low_amp, int life_time = 500) {
        vibrateStates[index] = {high_freq, low_freq, high_amp, low_amp, life_time, false, std::chrono::system_clock::now()};
    };

    ButtonState Joystick::GetButtonState(int32_t joystickIndex, int32_t buttonIndex) const {
        if (buttonIndex < 0) return ButtonState::Free;
        
        if (currentHit[joystickIndex][buttonIndex] && preHit[joystickIndex][buttonIndex]) return ButtonState::Hold;
        else if (!currentHit[joystickIndex][buttonIndex] && preHit[joystickIndex][buttonIndex]) return ButtonState::Release;
        else if (currentHit[joystickIndex][buttonIndex] && !preHit[joystickIndex][buttonIndex]) return ButtonState::Push;
        else return ButtonState::Free;
    };

    ButtonState Joystick::GetButtonState(int32_t joystickIndex, JoystickButtonType type) const {
        
        auto jtype = GetJoystickType(joystickIndex);
        
        if(jtype == JoystickType::Other)
            return ButtonState::Free;
        
        if(jtype == JoystickType::JoyconL)
        {
            std::array<int, (int32_t)JoystickButtonType::Max> maps;
            maps.fill(-1);

            maps[(int32_t)JoystickButtonType::LeftDown] = 0;
            maps[(int32_t)JoystickButtonType::LeftRight] = 1;
            maps[(int32_t)JoystickButtonType::LeftLeft] = 2;
            maps[(int32_t)JoystickButtonType::LeftUp] = 3;
            
            maps[(int32_t)JoystickButtonType::L3] = 4;
            maps[(int32_t)JoystickButtonType::R3] = 5;
            
            maps[(int32_t)JoystickButtonType::Select] = 8;
            maps[(int32_t)JoystickButtonType::Start] = 9;
            maps[(int32_t)JoystickButtonType::LeftPush] = 10;
            maps[(int32_t)JoystickButtonType::RightPush] = 11;
            
            maps[(int32_t)JoystickButtonType::Home] = 12;
            maps[(int32_t)JoystickButtonType::Capture] = 13;
            
            maps[(int32_t)JoystickButtonType::L1] = 14;
            maps[(int32_t)JoystickButtonType::L2] = 15;
            
            
            return GetButtonState(joystickIndex, maps[(int32_t)type]);
        }
        
        return ButtonState::Hold;
    };
    
    float Joystick::GetAxisState(int32_t joystickIndex, int32_t axisIndex) const {
        if (axisIndex < 0) return 0;
        
        return currentAxis[joystickIndex][axisIndex];
    };
    
    float Joystick::GetAxisState(int32_t joystickIndex, JoystickAxisType type) const {
        
        auto jtype = GetJoystickType(joystickIndex);
        if (jtype == JoystickType::Other) return 0;
        
        if (jtype == JoystickType::JoyconL)
        {
            
            return GetAxisState(joystickIndex, (int32_t)type);
        }
        
        return 0.0f;
    };
    
    JoystickType Joystick::GetJoystickType(int32_t index) const {
        return types[index];
    };
    
    wchar_t * Joystick::GetJoystickName(int16_t index) const {
        return names[index];
    }
    

}
