/*
  Copyright (c) 2019 Arnix Chen All right reserved.
  This is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public
  License as published by the Free Software Foundation; either
  version 3.0 of the License, or (at your option) any later version.
  This software is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.
  You should have received a copy of the GNU General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Modified 05-03-2019 by Arnix Chen
*/

#ifndef __TONE_FIX_TABLE__
#define __TONE_FIX_TABLE__

#define TONE_FIX_ENTRY  (const toneFixEntry PROGMEM)

const toneFixEntry toneFixTable[] PROGMEM = {
  TONE_FIX_ENTRY {{"au6", "xl3", "gj3"}, {"au3", "xl6", "gj3"}, {1, -1, 0}}, // 米老鼠
  TONE_FIX_ENTRY {{"aj6", "xl3", "cj3"}, {"aj3", "xl6", "cj3"}, {1, -1, 0}}, // 母老虎
  TONE_FIX_ENTRY {{"286", "hl3", "el3"}, {"283", "hl6", "el3"}, {1, -1, 0}}, // 打草稿
  TONE_FIX_ENTRY {{"wj6", "dj3", "gjo3"}, {"wj3", "dj6", "gjo3"}, {1, -1, 0}}, // 吐苦水
  TONE_FIX_ENTRY {{"su6", "su3", "j3"}, {"su.", "su.", "j3"}, { -1, -2, 0}}, // 扭扭舞
  TONE_FIX_ENTRY {{"sm6", "5j3", "rul3"}, {"sm3", "5j6", "rul3"}, {1, -1, 0}}, // 女主角
  TONE_FIX_ENTRY {{"xl6", "s93", "s93"}, {"xl6", "s93", "s97"}, {0, 0, 2}}, // 老奶奶
  TONE_FIX_ENTRY {{"xl6", "ej3", "2j/3"}, {"xl3", "ej6", "2j/3"}, {1, -1, 0}}, // 老古董
  TONE_FIX_ENTRY {{"xu6", "wu,3", "ej93"}, {"xu3", "wu,6", "ej93"}, {1, -1, 0}}, // 李鐵拐
  TONE_FIX_ENTRY {{"xu6", "yj/3", "wj/3"}, {"xu3", "yj/6", "wj/3"}, {1, -1, 0}}, // 李總統
  TONE_FIX_ENTRY {{"rul6", "2u3", "103"}, {"rul3", "2u6", "103"}, {1, -1, 0}}, // 腳底板
  TONE_FIX_ENTRY {{"rul6", "53", "ru83"}, {"rul3", "56", "ru83"}, {1, -1, 0}}, // 腳指甲
  TONE_FIX_ENTRY {{"ru;6", "yj/3", "wj/3"}, {"ru;3", "yj/6", "wj/3"}, {1, -1, 0}}, // 蔣總統
  TONE_FIX_ENTRY {{"fu/6", "rm3", "g.3"}, {"fu/3", "rm6", "g.3"}, {1, -1, 0}}, // 請舉手
  TONE_FIX_ENTRY {{"vu,6", "vul3", "103"}, {"vu,3", "vul6", "103"}, {1, -1, 0}}, // 血小板
  TONE_FIX_ENTRY {{"vul6", "1l3", "1l3"}, {"vul6", "1l3", "1l7"}, {0, 0, 2}}, // 小寶寶
  TONE_FIX_ENTRY {{"vul6", "1ul3", "ru,3"}, {"vul3", "1ul6", "ru,3"}, {1, -1, 0}}, // 小表姊
  TONE_FIX_ENTRY {{"vul6", "1ul3", "nl3"}, {"vul3", "1ul6", "nl3"}, {1, -1, 0}}, // 小表嫂
  TONE_FIX_ENTRY {{"vul6", "a83", "u3"}, {"vul3", "a86", "u3"}, {1, -1, 0}}, // 小螞蟻
  TONE_FIX_ENTRY {{"vul6", "aj3", "53"}, {"vul3", "aj6", "53"}, {1, -1, 0}}, // 小拇指
  TONE_FIX_ENTRY {{"vul6", "2l3", "m3"}, {"vul3", "2l6", "m3"}, {1, -1, 0}}, // 小島嶼
  TONE_FIX_ENTRY {{"vul6", "sm3", "y3"}, {"vul3", "sm6", "y3"}, {1, -1, 0}}, // 小女子
  TONE_FIX_ENTRY {{"vul6", "xl3", "cj3"}, {"vul3", "xl6", "cj3"}, {1, -1, 0}}, // 小老虎
  TONE_FIX_ENTRY {{"vul6", "xl3", "gj3"}, {"vul3", "xl6", "gj3"}, {1, -1, 0}}, // 小老鼠
  TONE_FIX_ENTRY {{"vul6", "xu83", "d.3"}, {"vul3", "xu86", "d.3"}, {1, -1, 0}}, // 小倆口
  TONE_FIX_ENTRY {{"vul6", "xu;3", "d.3"}, {"vul3", "xu;6", "d.3"}, {1, -1, 0}}, // 小兩口
  TONE_FIX_ENTRY {{"vul6", "e.3", "e.3"}, {"vul6", "e.3", "e.7"}, {0, 0, 2}}, // 小狗狗
  TONE_FIX_ENTRY {{"vul6", "ej3", "g.3"}, {"vul3", "ej6", "g.3"}, {1, -1, 0}}, // 小鼓手
  TONE_FIX_ENTRY {{"vul6", "cji3", "y3"}, {"vul6", "cji3", "y7"}, {0, 0, 2}}, // 小夥子
  TONE_FIX_ENTRY {{"vul6", "ru,3", "ru,3"}, {"vul6", "ru,3", "ru,7"}, {0, 0, 2}}, // 小姊姊
  TONE_FIX_ENTRY {{"vul6", "yj3", "5;3"}, {"vul3", "yj6", "5;3"}, {1, -1, 0}}, // 小組長
  TONE_FIX_ENTRY {{"vul6", "-3", "2ji3"}, {"vul6", "-3", "2ji7"}, {0, 0, 2}}, // 小耳朵
  TONE_FIX_ENTRY {{"vul6", "u3", "y3"}, {"vul6", "u3", "y7"}, {0, 0, 2}}, // 小椅子
  TONE_FIX_ENTRY {{"56", "xl3", "cj3"}, {"53", "xl6", "cj3"}, {1, -1, 0}}, // 紙老虎
  TONE_FIX_ENTRY {{"56", "dk3", "u3"}, {"53", "dk6", "u3"}, {1, -1, 0}}, // 只可以
  TONE_FIX_ENTRY {{"tl6", "au3", "zp3"}, {"tl3", "au6", "zp3"}, {1, -1, 0}}, // 炒米粉
  TONE_FIX_ENTRY {{"g.6", "53", "ru83"}, {"g.3", "56", "ru83"}, {1, -1, 0}}, // 手指甲
  TONE_FIX_ENTRY {{"gjo6", "1u3", "y93"}, {"gjo3", "1u6", "y93"}, {1, -1, 0}}, // 水筆仔
  TONE_FIX_ENTRY {{"yji6", "g.3", "5;3"}, {"yji3", "g.6", "5;3"}, {1, -1, 0}}, // 左手掌
  TONE_FIX_ENTRY {{"yj/6", "5j3", "1u3"}, {"yj/3", "5j6", "1u3"}, {1, -1, 0}}, // 總主筆
  TONE_FIX_ENTRY {{"yj/6", "nji3", "up3"}, {"yj/3", "nji6", "up3"}, {1, -1, 0}}, // 總索引
  TONE_FIX_ENTRY {{"n6", "103", "103"}, {"n3", "106", "103"}, {1, -1, 0}}, // 死板板
  TONE_FIX_ENTRY {{"-6", "2j3", "bj03"}, {"-3", "2ji", "bj03"}, {1, -2, 0}}, // 耳朵軟
  TONE_FIX_ENTRY {{"u,6", "eji3", "y3"}, {"u,6", "eji3", "y7"}, {0, 0, 2}}, // 野果子
  TONE_FIX_ENTRY {{"u,6", "dk3", "u3"}, {"u,3", "dk6", "u3"}, {1, -1, 0}}, // 也可以
  TONE_FIX_ENTRY {{"ul6", "-3", "2ji3"}, {"ul6", "-3", "2ji7"}, {0, 0, 2}}, // 咬耳朵
  TONE_FIX_ENTRY {{"ji6", "53", "u.3"}, {"ji3", "56", "u.3"}, {1, -1, 0}}, // 我只有
  TONE_FIX_ENTRY {{"ji6", "u,3", "u.3"}, {"ji3", "u,6", "u.3"}, {1, -1, 0}}, // 我也有
  TONE_FIX_ENTRY {{"ji6", "cp3", "cl3"}, {"ji3", "cp6", "cl3"}, {1, -1, 0}}, // 我很好
  TONE_FIX_ENTRY {{"ji6", "cp3", "vu;3"}, {"ji3", "cp6", "vu;3"}, {1, -1, 0}}, // 我很想
  TONE_FIX_ENTRY {{"ji6", "dk3", "u3"}, {"ji3", "dk6", "u3"}, {1, -1, 0}}, // 我可以
  TONE_FIX_ENTRY {{"su6", "cp3", "cl3"}, {"su3", "cp6", "cl3"}, {1, -1, 0}}, // 你很好
  TONE_FIX_ENTRY {{"su6", "cp3", "vu;3"}, {"su3", "cp6", "vu;3"}, {1, -1, 0}}, // 你很想
  TONE_FIX_ENTRY {{"su6", "dk3", "u3"}, {"su3", "dk6", "u3"}, {1, -1, 0}}, // 你可以

  TONE_FIX_ENTRY { {"", "", ""}, {"", "", ""}}, // dummy entry
};

#endif
