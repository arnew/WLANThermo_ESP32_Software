/***************************************************
    Copyright (C) 2020  Martin Koerner

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    HISTORY: Please refer Github History

****************************************************/
#include "lvPitmaster.h"
#include "lvScreen.h"
#include "system/SystemBase.h"
#include "Settings.h"

LV_FONT_DECLARE(Font_Nano_h24);
LV_FONT_DECLARE(Font_Roboto_Medium_h80);

static Pitmaster *lvPitmaster_pitmaster = NULL;

extern const uint16_t DisplayTftStartScreenImg[25400];

static lvPitmasterType lvPitmaster = {NULL};

void lvPitmaster_TabLimitInc(lv_obj_t *obj, lv_event_t event)
{
  if ((LV_EVENT_SHORT_CLICKED == event) || LV_EVENT_LONG_PRESSED_REPEAT == event)
  {
    lv_spinbox_increment((lv_obj_t *)lv_obj_get_user_data(obj));
  }
}

void lvPitmaster_TabLimitDec(lv_obj_t *obj, lv_event_t event)
{
  if ((LV_EVENT_SHORT_CLICKED == event) || LV_EVENT_LONG_PRESSED_REPEAT == event)
  {
    lv_spinbox_decrement((lv_obj_t *)lv_obj_get_user_data(obj));
  }
}

void lvPitmaster_saveTemperature(void)
{
  float value;

  value = ((float)lv_spinbox_get_value(lvPitmaster.spinbox)) / 10.0f;
  lvPitmaster_pitmaster->setTargetTemperature(value);

  gSystem->pitmasters.saveConfig();
}

void lvPitmaster_BtnClose(lv_obj_t *obj, lv_event_t event)
{
  if (LV_EVENT_CLICKED == event)
  {
    lvPitmaster_saveTemperature();
    lvScreen_Open(lvScreenType::Home);
  }
}

void lvPitmaster_Create(void *userData)
{
  lvPitmaster_pitmaster = gSystem->pitmasters[0];

  /* create screen for update */
  lvPitmaster.screen = lv_obj_create(NULL, NULL);

  lv_obj_t *cont = lv_cont_create(lvPitmaster.screen, NULL);
  lv_cont_set_fit(cont, LV_FIT_PARENT);
  lv_cont_set_layout(cont, LV_LAYOUT_PRETTY_MID);
  lv_obj_set_style_local_border_width(cont, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
  lv_obj_set_style_local_radius(cont, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 10);

  lv_obj_t *btn = lv_btn_create(cont, NULL);
  lv_obj_set_event_cb(btn, lvPitmaster_BtnClose);
  lv_obj_t *label = lv_label_create(btn, NULL);
  lv_label_set_text(label, LV_SYMBOL_CLOSE);
  lv_obj_set_pos(btn, LV_DPX(335), LV_DPX(12));
  lv_obj_set_size(btn, LV_DPX(50), LV_DPX(35));

  lvPitmaster.spinbox = lv_spinbox_create(cont, NULL);
  lv_spinbox_set_range(lvPitmaster.spinbox, -200, 9989);
  lv_spinbox_set_digit_format(lvPitmaster.spinbox, 4, 3);
  lv_spinbox_set_value(lvPitmaster.spinbox, ((int32_t)lvPitmaster_pitmaster->getTargetTemperature()) * 10.0f);
  lv_spinbox_step_prev(lvPitmaster.spinbox);
  lv_obj_set_width(lvPitmaster.spinbox, 270);
  lv_obj_align(lvPitmaster.spinbox, NULL, LV_ALIGN_CENTER, 0, 20);
  lv_obj_set_style_local_text_font(lvPitmaster.spinbox, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, &Font_Roboto_Medium_h80);

  lv_obj_t *btnDec = lv_btn_create(cont, NULL);
  lv_obj_set_size(btnDec, 100, 50);
  lv_obj_set_style_local_value_str(btnDec, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_MINUS);
  lv_obj_set_user_data(btnDec, lvPitmaster.spinbox);
  lv_obj_set_event_cb(btnDec, lvPitmaster_TabLimitDec);

  lv_obj_t *btnInc = lv_btn_create(cont, NULL);
  lv_obj_set_size(btnInc, 100, 50);
  lv_obj_set_style_local_value_str(btnInc, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_PLUS);
  lv_obj_set_user_data(btnInc, lvPitmaster.spinbox);
  lv_obj_set_event_cb(btnInc, lvPitmaster_TabLimitInc);
  lvPitmaster_Update(true);

  lv_scr_load(lvPitmaster.screen);
}

void lvPitmaster_Update(boolean forceUpdate)
{
  // lv_spinbox_set_value(lvPitmaster.spinbox, ((int32_t)lvPitmaster_pitmaster->getTargetTemperature()) * 10.0f);
}

void lvPitmaster_Delete(void)
{
  lv_obj_del(lvPitmaster.screen);
}
