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

#define HUD_AREA_X2				0.9f
#define HUD_AREA_Y				0.1f
#define HUD_AREA_H				0.8f

#define HUD_FIELD_CONTENT_SIZE	0.95f

#define HUD_FONT_SIZE			14.0f

#define HUD_LBL_TIME_DAY_TEXT	"day:"
#define HUD_LBL_TIME_HOUR_TEXT	"at:"
#define HUD_LBL_MONEY_TEXT		"money:"
#define HUD_LBL_HOVER_TEXT		"field:"
#define HUD_BTN_PASS_TEXT		"->"

#define HUD_LBL_TIME_DAY_X				0.005f
#define HUD_LBL_TIME_DAY_Y				0.005f
#define HUD_LBL_TIME_DAY_VAL_X_DIST		0.01f			/* distance from previous widget on the left */
#define HUD_LBL_TIME_DAY_VAL_Y			HUD_LBL_TIME_DAY_Y
#define HUD_LBL_TIME_HOUR_X				HUD_LBL_TIME_DAY_X
#define HUD_LBL_TIME_HOUR_Y				(HUD_LBL_TIME_DAY_Y + 0.025f)
#define HUD_LBL_TIME_HOUR_VAL_X_DIST	HUD_LBL_TIME_DAY_VAL_X_DIST
#define HUD_LBL_TIME_HOUR_VAL_Y			HUD_LBL_TIME_HOUR_Y

#define HUD_LBL_MONEY_X				0.15f
#define HUD_LBL_MONEY_Y				HUD_LBL_TIME_DAY_Y
#define HUD_LBL_MONEY_VAL_X_DIST	HUD_LBL_TIME_DAY_VAL_X_DIST
#define HUD_LBL_MONEY_VAL_Y			HUD_LBL_MONEY_Y

#define HUD_LBL_HOVER_X				0.005f
#define HUD_LBL_HOVER_Y				0.075f
#define HUD_LBL_HOVER_X_VAL_X_DIST	0.025f
#define HUD_LBL_HOVER_X_VAL_Y		HUD_LBL_HOVER_Y
#define HUD_LBL_HOVER_Y_VAL_X_DIST	HUD_LBL_HOVER_X_VAL_X_DIST
#define HUD_LBL_HOVER_Y_VAL_Y		HUD_LBL_HOVER_X_VAL_Y

#define HUD_BTN_PASS_X	0.9f
#define HUD_BTN_PASS_Y	0.9f
#define HUD_BTN_PASS_W	0.05f
#define HUD_BTN_PASS_H	0.05f

#endif /* DEF_HUD_H */
