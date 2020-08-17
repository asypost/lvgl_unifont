# lvgl_unifont
unifont library for LittlevGL

# usage
1. clone or download this repo
2. change the FONT_FILE value defined in lv_font_unifont.h
3. include the lv_font_unifont.h
4. apply the font to your widget

```
lv_style_t style;
lv_style_init(&style);
lv_style_set_text_font(&style, LV_STATE_DEFAULT, &lv_font_unifont_16);
lv_obj_add_style(label, LV_LABEL_PART_MAIN, &style);
```

# font
you can download font file from [GNU Unifont Glyphs](http://unifoundry.com/unifont/index.html) or use the one I provided in this repo.

# font index generate tool
a font index tool can be found in tool directory,which you can generate font index for your own unifont file.
