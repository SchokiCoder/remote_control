/*
	remote_control
	Copyright (C) 2021	Andy Frank Schoknecht

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

#ifndef DEF_HUD_H
#define DEF_HUD_H

#define HUD_WIDGET_TEXT_MAX_LEN	64

/* area, fields */
static const float HUD_AREA_X2 =	0.9f;
static const float HUD_AREA_Y =		0.1f;
static const float HUD_AREA_H =		0.8f;

static const float HUD_FIELD_CONTENT_SIZE =	0.95f;

/* font */
static const float HUD_FONT_SIZE =	14.0f;

/* bars */
static const uint8_t HUD_BAR_COLOR_R = 20;
static const uint8_t HUD_BAR_COLOR_G = 20;
static const uint8_t HUD_BAR_COLOR_B = 20;
static const uint8_t HUD_BAR_COLOR_A = 200;

static const float HUD_BAR_TOP_X = 0.0f;
static const float HUD_BAR_TOP_Y = 0.0f;
static const float HUD_BAR_TOP_W = 100.0f;
static const float HUD_BAR_TOP_H_MARGIN = 0.001f;

static const float HUD_BAR_SIDE_X = 0.0f;
static const float HUD_BAR_SIDE_Y = 0.35f;
static const float HUD_BAR_SIDE_W = 0.10f;
static const float HUD_BAR_SIDE_H = 0.26f;

/* top bar widgets */
static const char HUD_LBL_TIME_DAY_TEXT[] =		"day:";
static const char HUD_LBL_TIME_HOUR_TEXT[] =	"at:";
static const char HUD_LBL_MONEY_TEXT[] =		"money:";

static const float HUD_LBL_TIME_DAY_X =				0.005f;
static const float HUD_LBL_TIME_DAY_Y =				HUD_BAR_TOP_H_MARGIN;

static const float HUD_LBL_TIME_DAY_VAL_X_DIST =	0.01f;
static const float HUD_LBL_TIME_DAY_VAL_Y =			HUD_LBL_TIME_DAY_Y;

static const float HUD_LBL_TIME_HOUR_X_DIST =		0.03f;
static const float HUD_LBL_TIME_HOUR_Y =			HUD_BAR_TOP_H_MARGIN;

static const float HUD_LBL_TIME_HOUR_VAL_X_DIST =	HUD_LBL_TIME_DAY_VAL_X_DIST;
static const float HUD_LBL_TIME_HOUR_VAL_Y =		HUD_LBL_TIME_HOUR_Y;

static const float HUD_LBL_MONEY_X_DIST 	=		HUD_LBL_TIME_HOUR_X_DIST;
static const float HUD_LBL_MONEY_Y =				HUD_BAR_TOP_H_MARGIN;

static const float HUD_LBL_MONEY_VAL_X_DIST =		HUD_LBL_TIME_DAY_VAL_X_DIST;
static const float HUD_LBL_MONEY_VAL_Y =			HUD_LBL_MONEY_Y;

/* side bar widgets */
static const char HUD_BTN_PASS_TEXT[] =	"->";

static const float HUD_BTN_PASS_X =	0.02f;
static const float HUD_BTN_PASS_Y =	HUD_BAR_SIDE_Y + 0.02f;
static const float HUD_BTN_PASS_W =	0.06f;
static const float HUD_BTN_PASS_H =	0.06f;

static const float HUD_BTN_CONSTRUCT_X =	HUD_BTN_PASS_X;
static const float HUD_BTN_CONSTRUCT_Y =	HUD_BTN_PASS_Y + HUD_BTN_PASS_H + 0.025f;
static const float HUD_BTN_CONSTRUCT_W =	HUD_BTN_PASS_W;
static const float HUD_BTN_CONSTRUCT_H =	HUD_BTN_PASS_H;

static const float HUD_BTN_DECONSTRUCT_X =	HUD_BTN_PASS_X;
static const float HUD_BTN_DECONSTRUCT_Y =	HUD_BTN_CONSTRUCT_Y + HUD_BTN_CONSTRUCT_H + 0.025f;
static const float HUD_BTN_DECONSTRUCT_W =	HUD_BTN_CONSTRUCT_W;
static const float HUD_BTN_DECONSTRUCT_H =	HUD_BTN_CONSTRUCT_H;

/* bot bar widgets */
static const float HUD_BTN_CONSTRUCT_QUARRY_X =	0.05f;
static const float HUD_BTN_CONSTRUCT_QUARRY_Y =	0.90f;
static const float HUD_BTN_CONSTRUCT_QUARRY_W =	0.05f;
static const float HUD_BTN_CONSTRUCT_QUARRY_H =	0.05f;

/* deconstruct hover color */
static const uint8_t HUD_DECONSTRUCT_HOVER_R = 255;
static const uint8_t HUD_DECONSTRUCT_HOVER_G = 100;
static const uint8_t HUD_DECONSTRUCT_HOVER_B = 100;
static const uint8_t HUD_DECONSTRUCT_HOVER_A = 150;

#endif /* DEF_HUD_H */
