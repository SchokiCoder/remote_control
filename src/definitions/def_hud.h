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

#define HUD_TIME_DAY_LABEL_TEXT		"day:"
#define HUD_TIME_HOUR_LABEL_TEXT	"at:"
#define HUD_MONEY_LABEL_TEXT		"money:"
#define HUD_HOVER_LABEL_TEXT		"field:"

#define HUD_TIME_DAY_LABEL_X		0.005f
#define HUD_TIME_DAY_LABEL_Y		0.005f
#define HUD_TIME_DAY_X_DIST			0.01f					/* distance from previous widget on the left */
#define HUD_TIME_DAY_Y				HUD_TIME_DAY_LABEL_Y
#define HUD_TIME_HOUR_LABEL_X		HUD_TIME_DAY_LABEL_X
#define HUD_TIME_HOUR_LABEL_Y		(HUD_TIME_DAY_LABEL_Y + 0.025f)
#define HUD_TIME_HOUR_X_DIST		HUD_TIME_DAY_X_DIST
#define HUD_TIME_HOUR_Y				HUD_TIME_HOUR_LABEL_Y

#define HUD_MONEY_LABEL_X	0.15f
#define HUD_MONEY_LABEL_Y	HUD_TIME_DAY_LABEL_Y
#define HUD_MONEY_X_DIST	HUD_TIME_DAY_X_DIST
#define HUD_MONEY_Y			HUD_MONEY_LABEL_Y

#define HUD_HOVER_LABEL_X	0.005f
#define HUD_HOVER_LABEL_Y	0.075f
#define HUD_HOVER_X_X_DIST	0.025f
#define HUD_HOVER_X_Y		HUD_HOVER_LABEL_Y
#define HUD_HOVER_Y_X_DIST	HUD_HOVER_X_X_DIST
#define HUD_HOVER_Y_Y		HUD_HOVER_X_Y

#endif /* DEF_HUD_H */
