/*
    remote_control
    Copyright (C) 2021  Andy Frank Schoknecht

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"
#include "path.h"
#include "config.h"

int32_t load_config(struct Config* p_cfg)
{
    FILE *f;
    char filepath[FILEPATH_MAX_LEN] = "";
    int32_t buf;
    uint32_t line = 0;
    uint32_t setting_len = 0;
    uint32_t value_len = 0;
    bool read_setting = true;
    char cfg_settings[CONFIG_MAX_LINES][CONFIG_MAX_SETTING_LEN];
    char cfg_values[CONFIG_MAX_LINES][CONFIG_MAX_VALUE_LEN];

    //get path
    if (get_config_path(filepath) != 0)
        return 1;

    //open file
    f = fopen(filepath, "r");

    if (f == NULL)
    {
        printf(MSG_WARN_CONFIG_LOAD);
        return 2;
    }

    //read
    while ((buf = fgetc(f)) != EOF)
    {
        switch (buf)
        {
        case ' ':
            //ignore spaces
        break;

        case '=':
            //start reading value, add '\0' to setting
            read_setting = false;
            cfg_settings[line][setting_len] = '\0';
            setting_len++;
        break;

        case '\n':
            //add '\0' to value
            cfg_values[line][value_len] = '\0';
            value_len++;    
            
            //jump to next line, reset values
            line++;
            read_setting = true;
            setting_len = 0;
            value_len = 0;
        break;

        default:
            //if comma did not yet occur
            if (read_setting == true)
            {
                //read setting-name
                cfg_settings[line][setting_len] = (unsigned char) buf;
                setting_len++;
            }
            else
            {
                //else read value
                cfg_values[line][value_len] = (unsigned char) buf;
                value_len++;
            }
        break;
        }
    }

    //if last line is empty, decrement line counter
    if (strcmp(cfg_settings[line], "") == 0)
    {
        line--;
    }

    //convert strings into values
    for (uint32_t i = 0; i < (line + 1); i++)
    {
        if (strcmp(cfg_settings[i], CFG_SETTING_GFX_FRAMERATE) == 0)
        {
            p_cfg->gfx_framerate = strtof(cfg_values[i], NULL);
        }
        else if (strcmp(cfg_settings[i], CFG_SETTING_GFX_WINDOW_X) == 0)
        {
            p_cfg->gfx_window_x = strtof(cfg_values[i], NULL);
        }
        else if(strcmp(cfg_settings[i], CFG_SETTING_GFX_WINDOW_Y) == 0)
        {
            p_cfg->gfx_window_y = strtof(cfg_values[i], NULL);
        }
        else if(strcmp(cfg_settings[i], CFG_SETTING_GFX_WINDOW_W) == 0)
        {
            p_cfg->gfx_window_w = strtof(cfg_values[i], NULL);
        }
        else if(strcmp(cfg_settings[i], CFG_SETTING_GFX_WINDOW_H) == 0)
        {
            p_cfg->gfx_window_h = strtof(cfg_values[i], NULL);
        }
        else if (strcmp(cfg_settings[i], CFG_SETTING_FIELD_BORDER_RED) == 0)
        {
            p_cfg->field_border_red = strtoul(cfg_values[i], NULL, 10);
        }
        else if (strcmp(cfg_settings[i], CFG_SETTING_FIELD_BORDER_GREEN) == 0)
        {
            p_cfg->field_border_green = strtoul(cfg_values[i], NULL, 10);
        }
        else if (strcmp(cfg_settings[i], CFG_SETTING_FIELD_BORDER_BLUE) == 0)
        {
            p_cfg->field_border_blue = strtoul(cfg_values[i], NULL, 10);
        }
        else if (strcmp(cfg_settings[i], CFG_SETTING_FIELD_BORDER_ALPHA) == 0)
        {
            p_cfg->field_border_alpha = strtoul(cfg_values[i], NULL, 10);
        }
        else
        {
            printf(MSG_WARN_CONFIG_UNKNOWN_SETTING, cfg_settings[i]);
        }
    }

    fclose(f);

    return 0;
}

int32_t save_config(struct Config* p_cfg)
{
    FILE *f;
    char filepath[FILEPATH_MAX_LEN] = "";
    const char* delim = " = ";

    //get path
    if (get_config_path(filepath) != 0)
        return 1;

    //open
    f = fopen(filepath, "w");

    if (f == NULL)
    {
        printf(MSG_WARN_CONFIG_SAVE);
        return 2;
    }

    //write
    fputs(CFG_SETTING_GFX_FRAMERATE, f);
    fputs(delim, f);
    fprintf(f, "%f", p_cfg->gfx_framerate);
    fputc('\n', f);
    
    fputs(CFG_SETTING_GFX_WINDOW_X, f);
    fputs(delim, f);
    fprintf(f, "%f", p_cfg->gfx_window_x);
    fputc('\n', f);

    fputs(CFG_SETTING_GFX_WINDOW_Y, f);
    fputs(delim, f);
    fprintf(f, "%f", p_cfg->gfx_window_y);
    fputc('\n', f);

    fputs(CFG_SETTING_GFX_WINDOW_W, f);
    fputs(delim, f);
    fprintf(f, "%f", p_cfg->gfx_window_w);
    fputc('\n', f);

    fputs(CFG_SETTING_GFX_WINDOW_H, f);
    fputs(delim, f);
    fprintf(f, "%f", p_cfg->gfx_window_h);
    fputc('\n', f);

    fputs(CFG_SETTING_FIELD_BORDER_RED, f);
    fputs(delim, f);
    fprintf(f, "%u", p_cfg->field_border_red);
    fputc('\n', f);

    fputs(CFG_SETTING_FIELD_BORDER_GREEN, f);
    fputs(delim, f);
    fprintf(f, "%u", p_cfg->field_border_green);
    fputc('\n', f);

    fputs(CFG_SETTING_FIELD_BORDER_BLUE, f);
    fputs(delim, f);
    fprintf(f, "%u", p_cfg->field_border_blue);
    fputc('\n', f);

    fputs(CFG_SETTING_FIELD_BORDER_ALPHA, f);
    fputs(delim, f);
    fprintf(f, "%u", p_cfg->field_border_alpha);
    fputc('\n', f);

    fclose(f);
    
    return 0;
}
