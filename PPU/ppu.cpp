#include "ppu.h"

PPU::PPU()
{}
// PPU


uint8_t PPU::cpu_read(uint16_t addrss){
    uint8_t data= {};

    switch (addrss) {

        case 0x0:{ // ctrl
            break;
        }

        case 0x1:{ // mask
            break;
        }
        case 0x2:{ // status
            break;
        }
        case 0x3:{ // QAM address
            break;
        }
        case 0x4:{ // QAM data
            break;
        }
        case 0x5:{ // Scroll
            break;
        }
        case 0x6:{ // PPU address
            break;
        }
        case 0x7:{ // PPU data
            break;
        }

    }
    return data;
};
void  PPU::cpu_write(uint16_t addrss, uint8_t data){

    switch (addrss) {

        case 0x0:{ // ctrl
            break;
        }

        case 0x1:{ // mask
            break;
        }
        case 0x2:{ // status
            break;
        }
        case 0x3:{ // QAM address
            break;
        }
        case 0x4:{ // QAM data
            break;
        }
        case 0x5:{ // Scroll
            break;
        }
        case 0x6:{ // PPU address
            break;
        }
        case 0x7:{ // PPU data
            break;
        }

    }

};


//PPU


uint8_t  PPU::ppu_read(uint16_t addrss){
    uint8_t data= {};
    addrss &= 0x3fff;

    return data;

};
void  PPU::ppu_write(uint16_t addrss, uint8_t data){
    addrss &= 0x3fff;
};
