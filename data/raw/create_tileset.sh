#!/bin/sh

# montage go*.png -background none -tile 4x2 -geometry 224x364 11_skeleton.png

montage \
  building_1_0.png building_1_1.png building_2_0.png building_2_1.png building_3_0.png building_3_1.png building_bank_0.png building_bank_1.png \
  building_1_2.png building_1_3.png building_2_2.png building_2_3.png building_3_2.png building_3_3.png building_bank_2.png building_bank_3.png \
  building_gun_store_0.png building_gun_store_1.png building_home_1_0.png building_home_1_1.png building_home_2_0.png building_home_2_1.png grass1.png grass2.png \
  building_gun_store_2.png building_gun_store_3.png building_home_1_2.png building_home_1_3.png building_home_2_2.png building_home_2_3.png grass3.png grass4.png \
  roadblock_1.png roadblock_2.png roadblock_3.png roadblock_4.png roadblock_7.png roadblock_8.png roadblock_5.png roadblock_6.png \
  road_l_1.png road_l_2.png road_l_3.png road_l_4.png road_cross.png road_hor.png road_ver.png \
  -background none \
  -tile 8x8 \
  -geometry 256x256 \
  tileset.png
