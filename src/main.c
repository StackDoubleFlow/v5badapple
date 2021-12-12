#include "main.h"
#include "frame_data.h"
#include "picojpeg.h"
#include "display/lvgl.h"

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {}

unsigned char callback(unsigned char* pBuf, unsigned char buf_size, unsigned char *pBytes_actually_read, uint8_t **pCallback_data) {
    memcpy(pBuf, *pCallback_data, buf_size);
    *pBytes_actually_read = buf_size;
    *pCallback_data += buf_size;
    return 0;
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
    printf("Opening data file...\n");
    fflush(stdout);
    FILE *data_file = fopen("/usd/frame_data.bin", "r");
    if (data_file == NULL) {
        printf("Error opening data file (error code %i)\n", errno);
        return;
    }
    printf("Sucessfully opened data file\n");
    fflush(stdout);
    uint8_t *frame_data = malloc(5767288);
    printf("Sucessfully allocated frame data\n");
    fflush(stdout);
    fread(frame_data, 5767288, 1, data_file);
    fclose(data_file);
    printf("Sucessfully loaded frame data\n");
    fflush(stdout);

    uint8_t *buf = malloc(480 * 272 * LV_COLOR_DEPTH / 8);
    lv_obj_t *frame_obj = lv_img_create(lv_scr_act(), NULL);
    lv_img_dsc_t frame_dsc = {
        .header.always_zero = 0,
        .header.w = 480,
        .header.h = 272,
        .header.cf = LV_IMG_CF_TRUE_COLOR,
        .data_size = 480 * 272 * LV_COLOR_DEPTH / 8,
        .data = buf,
    };
    lv_img_set_src(frame_obj, &frame_dsc);
    printf("Created image object\n");

    unsigned char err;
    
    for (int i = 0; i < 2191; i++) {
        // printf("Rendering frame %i\n", i);
        pjpeg_image_info_t imgInfo;
        uint8_t *ptr = (uint8_t *)&frame_data[frame_data_idx[i]];
        err = pjpeg_decode_init(&imgInfo, (pjpeg_need_bytes_callback_t)callback, &ptr, 0);
        if (err) {
            printf("pjpeg_decode_init failed with error code %i\n", err);
        }

        for (int row = 0; row < imgInfo.m_MCUSPerCol; row++) {
            for (int col = 0; col < imgInfo.m_MCUSPerRow; col++) {
                err = pjpeg_decode_mcu();
                if (err) {
                    printf("pjpeg_decode_mcu failed with error code %i\n", err);
                }
                
                int yOffset = row * 16;
                int xOffset = col * 16;
                int mcuBufOffset = 0;
                for (int y = 0; y < 8; y++) {
                    for (int x = 0; x < 8; x++) {
                        int bufIdx = ((yOffset + y) * imgInfo.m_width) + (xOffset + x);
                        buf[bufIdx * 4] = imgInfo.m_pMCUBufR[mcuBufOffset + (y * 8 + x)];
                        buf[bufIdx * 4 + 1] = imgInfo.m_pMCUBufG[mcuBufOffset + (y * 8 + x)];
                        buf[bufIdx * 4 + 2] = imgInfo.m_pMCUBufB[mcuBufOffset + (y * 8 + x)];
                    }
                }

                xOffset += 8;
                mcuBufOffset = 64;
                for (int y = 0; y < 8; y++) {
                    for (int x = 0; x < 8; x++) {
                        int bufIdx = ((yOffset + y) * imgInfo.m_width) + (xOffset + x);
                        buf[bufIdx * 4] = imgInfo.m_pMCUBufR[mcuBufOffset + (y * 8 + x)];
                        buf[bufIdx * 4 + 1] = imgInfo.m_pMCUBufG[mcuBufOffset + (y * 8 + x)];
                        buf[bufIdx * 4 + 2] = imgInfo.m_pMCUBufB[mcuBufOffset + (y * 8 + x)];
                    }
                }

                xOffset -= 8;
                yOffset += 8;
                mcuBufOffset = 128;
                for (int y = 0; y < 8; y++) {
                    for (int x = 0; x < 8; x++) {
                        int bufIdx = ((yOffset + y) * imgInfo.m_width) + (xOffset + x);
                        buf[bufIdx * 4] = imgInfo.m_pMCUBufR[mcuBufOffset + (y * 8 + x)];
                        buf[bufIdx * 4 + 1] = imgInfo.m_pMCUBufG[mcuBufOffset + (y * 8 + x)];
                        buf[bufIdx * 4 + 2] = imgInfo.m_pMCUBufB[mcuBufOffset + (y * 8 + x)];
                    }
                }

                xOffset += 8;
                mcuBufOffset = 192;
                for (int y = 0; y < 8; y++) {
                    for (int x = 0; x < 8; x++) {
                        int bufIdx = ((yOffset + y) * imgInfo.m_width) + (xOffset + x);
                        buf[bufIdx * 4] = imgInfo.m_pMCUBufR[mcuBufOffset + (y * 8 + x)];
                        buf[bufIdx * 4 + 1] = imgInfo.m_pMCUBufG[mcuBufOffset + (y * 8 + x)];
                        buf[bufIdx * 4 + 2] = imgInfo.m_pMCUBufB[mcuBufOffset + (y * 8 + x)];
                    }
                }
            }
        }

        lv_img_set_src(frame_obj, &frame_dsc);
        delay(100);
    }

    printf("Playback completed");
    fflush(stdout);
    while (true) {
        delay(2);
    }
}
