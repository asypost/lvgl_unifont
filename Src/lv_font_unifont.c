/*
 * lv_font_unifont.c
 *
 *  Created on: 2020年7月30日
 *      Author: Shell
 */

#include "lv_font_unifont.h"
#include <string.h>
#include <assert.h>

static lv_fs_file_t font_file;
static _Bool font_file_opened = false;

static uint8_t font_glyph[32] = { 0 };

typedef struct {
	uint32_t start;
	uint32_t end;
	uint8_t size;
} __attribute__((packed)) _UnifontIndex;

static _UnifontIndex _font_index[] = {  {.start = 0x0000,.end = 0x001f,.size = 70 },
		{.start = 0x0020,.end = 0x007e,.size = 38 },
		{.start = 0x007f,.end = 0x009f,.size = 70 },
		{.start = 0x00a0,.end = 0x00ac,.size = 38 },
		{.start = 0x00ad,.end = 0x00ad,.size = 70 },
		{.start = 0x00ae,.end = 0x034e,.size = 38 },
		{.start = 0x034f,.end = 0x034f,.size = 70 },
		{.start = 0x0350,.end = 0x035b,.size = 38 },
		{.start = 0x035c,.end = 0x0362,.size = 70 },
		{.start = 0x0363,.end = 0x0377,.size = 38 },
		{.start = 0x0378,.end = 0x0379,.size = 70 },
		{.start = 0x037a,.end = 0x037f,.size = 38 },
		{.start = 0x0380,.end = 0x0383,.size = 70 },
		{.start = 0x0384,.end = 0x038a,.size = 38 },
		{.start = 0x038b,.end = 0x038b,.size = 70 },
		{.start = 0x038c,.end = 0x038c,.size = 38 },
		{.start = 0x038d,.end = 0x038d,.size = 70 },
		{.start = 0x038e,.end = 0x03a1,.size = 38 },
		{.start = 0x03a2,.end = 0x03a2,.size = 70 },
		{.start = 0x03a3,.end = 0x0487,.size = 38 },
		{.start = 0x0488,.end = 0x0489,.size = 70 },
		{.start = 0x048a,.end = 0x0529,.size = 38 },
		{.start = 0x052a,.end = 0x052b,.size = 70 },
		{.start = 0x052c,.end = 0x052f,.size = 38 },
		{.start = 0x0530,.end = 0x0530,.size = 70 },
		{.start = 0x0531,.end = 0x0556,.size = 38 },
		{.start = 0x0557,.end = 0x0558,.size = 70 },
		{.start = 0x0559,.end = 0x058a,.size = 38 },
		{.start = 0x058b,.end = 0x058e,.size = 70 },
		{.start = 0x058f,.end = 0x058f,.size = 38 },
		{.start = 0x0590,.end = 0x0590,.size = 70 },
		{.start = 0x0591,.end = 0x05c7,.size = 38 },
		{.start = 0x05c8,.end = 0x05cf,.size = 70 },
		{.start = 0x05d0,.end = 0x05ea,.size = 38 },
		{.start = 0x05eb,.end = 0x05ee,.size = 70 },
		{.start = 0x05ef,.end = 0x05f4,.size = 38 },
		{.start = 0x05f5,.end = 0x0608,.size = 70 },
		{.start = 0x0609,.end = 0x060a,.size = 38 },
		{.start = 0x060b,.end = 0x060b,.size = 70 },
		{.start = 0x060c,.end = 0x060d,.size = 38 },
		{.start = 0x060e,.end = 0x060f,.size = 70 },
		{.start = 0x0610,.end = 0x0610,.size = 38 },
		{.start = 0x0611,.end = 0x0617,.size = 70 },
		{.start = 0x0618,.end = 0x061b,.size = 38 },
		{.start = 0x061c,.end = 0x061e,.size = 70 },
		{.start = 0x061f,.end = 0x0655,.size = 38 },
		{.start = 0x0656,.end = 0x0656,.size = 70 },
		{.start = 0x0657,.end = 0x06a9,.size = 38 },
		{.start = 0x06aa,.end = 0x06aa,.size = 70 },
		{.start = 0x06ab,.end = 0x06d5,.size = 38 },
		{.start = 0x06d6,.end = 0x06da,.size = 70 },
		{.start = 0x06db,.end = 0x06db,.size = 38 },
		{.start = 0x06dc,.end = 0x06de,.size = 70 },
		{.start = 0x06df,.end = 0x06e1,.size = 38 },
		{.start = 0x06e2,.end = 0x06e3,.size = 70 },
		{.start = 0x06e4,.end = 0x06e7,.size = 38 },
		{.start = 0x06e8,.end = 0x06ed,.size = 70 },
		{.start = 0x06ee,.end = 0x06ff,.size = 38 },
		{.start = 0x0700,.end = 0x074f,.size = 70 },
		{.start = 0x0750,.end = 0x078f,.size = 38 },
		{.start = 0x0790,.end = 0x0790,.size = 70 },
		{.start = 0x0791,.end = 0x079c,.size = 38 },
		{.start = 0x079d,.end = 0x079f,.size = 70 },
		{.start = 0x07a0,.end = 0x07b1,.size = 38 },
		{.start = 0x07b2,.end = 0x07bf,.size = 70 },
		{.start = 0x07c0,.end = 0x07fa,.size = 38 },
		{.start = 0x07fb,.end = 0x07fc,.size = 70 },
		{.start = 0x07fd,.end = 0x07ff,.size = 38 },
		{.start = 0x0800,.end = 0x089f,.size = 70 },
		{.start = 0x08a0,.end = 0x08b4,.size = 38 },
		{.start = 0x08b5,.end = 0x08b5,.size = 70 },
		{.start = 0x08b6,.end = 0x08c7,.size = 38 },
		{.start = 0x08c8,.end = 0x08d2,.size = 70 },
		{.start = 0x08d3,.end = 0x08d9,.size = 38 },
		{.start = 0x08da,.end = 0x08df,.size = 70 },
		{.start = 0x08e0,.end = 0x08ff,.size = 38 },
		{.start = 0x0900,.end = 0x0e00,.size = 70 },
		{.start = 0x0e01,.end = 0x0e3a,.size = 38 },
		{.start = 0x0e3b,.end = 0x0e3e,.size = 70 },
		{.start = 0x0e3f,.end = 0x0e5a,.size = 38 },
		{.start = 0x0e5b,.end = 0x0e80,.size = 70 },
		{.start = 0x0e81,.end = 0x0e82,.size = 38 },
		{.start = 0x0e83,.end = 0x0e83,.size = 70 },
		{.start = 0x0e84,.end = 0x0e84,.size = 38 },
		{.start = 0x0e85,.end = 0x0e85,.size = 70 },
		{.start = 0x0e86,.end = 0x0e8a,.size = 38 },
		{.start = 0x0e8b,.end = 0x0e8b,.size = 70 },
		{.start = 0x0e8c,.end = 0x0ea3,.size = 38 },
		{.start = 0x0ea4,.end = 0x0ea4,.size = 70 },
		{.start = 0x0ea5,.end = 0x0ea5,.size = 38 },
		{.start = 0x0ea6,.end = 0x0ea6,.size = 70 },
		{.start = 0x0ea7,.end = 0x0ebd,.size = 38 },
		{.start = 0x0ebe,.end = 0x0ebf,.size = 70 },
		{.start = 0x0ec0,.end = 0x0ec4,.size = 38 },
		{.start = 0x0ec5,.end = 0x0ec5,.size = 70 },
		{.start = 0x0ec6,.end = 0x0ec6,.size = 38 },
		{.start = 0x0ec7,.end = 0x0ec7,.size = 70 },
		{.start = 0x0ec8,.end = 0x0ecd,.size = 38 },
		{.start = 0x0ece,.end = 0x0ecf,.size = 70 },
		{.start = 0x0ed0,.end = 0x0ed9,.size = 38 },
		{.start = 0x0eda,.end = 0x0edb,.size = 70 },
		{.start = 0x0edc,.end = 0x0edf,.size = 38 },
		{.start = 0x0ee0,.end = 0x0eff,.size = 70 },
		{.start = 0x0f00,.end = 0x0f01,.size = 38 },
		{.start = 0x0f02,.end = 0x0f03,.size = 70 },
		{.start = 0x0f04,.end = 0x0f15,.size = 38 },
		{.start = 0x0f16,.end = 0x0f18,.size = 70 },
		{.start = 0x0f19,.end = 0x0f39,.size = 38 },
		{.start = 0x0f3a,.end = 0x0f3b,.size = 70 },
		{.start = 0x0f3c,.end = 0x0f47,.size = 38 },
		{.start = 0x0f48,.end = 0x0f48,.size = 70 },
		{.start = 0x0f49,.end = 0x0f6c,.size = 38 },
		{.start = 0x0f6d,.end = 0x0f70,.size = 70 },
		{.start = 0x0f71,.end = 0x0f87,.size = 38 },
		{.start = 0x0f88,.end = 0x0f88,.size = 70 },
		{.start = 0x0f89,.end = 0x0f97,.size = 38 },
		{.start = 0x0f98,.end = 0x0f98,.size = 70 },
		{.start = 0x0f99,.end = 0x0fbc,.size = 38 },
		{.start = 0x0fbd,.end = 0x0fbd,.size = 70 },
		{.start = 0x0fbe,.end = 0x0fc4,.size = 38 },
		{.start = 0x0fc5,.end = 0x0fcd,.size = 70 },
		{.start = 0x0fce,.end = 0x0fcf,.size = 38 },
		{.start = 0x0fd0,.end = 0x0fd0,.size = 70 },
		{.start = 0x0fd1,.end = 0x0fd4,.size = 38 },
		{.start = 0x0fd5,.end = 0x109f,.size = 70 },
		{.start = 0x10a0,.end = 0x10c5,.size = 38 },
		{.start = 0x10c6,.end = 0x10c6,.size = 70 },
		{.start = 0x10c7,.end = 0x10c7,.size = 38 },
		{.start = 0x10c8,.end = 0x10cc,.size = 70 },
		{.start = 0x10cd,.end = 0x10cd,.size = 38 },
		{.start = 0x10ce,.end = 0x10cf,.size = 70 },
		{.start = 0x10d0,.end = 0x10ff,.size = 38 },
		{.start = 0x1100,.end = 0x1342,.size = 70 },
		{.start = 0x1343,.end = 0x1343,.size = 38 },
		{.start = 0x1344,.end = 0x1360,.size = 70 },
		{.start = 0x1361,.end = 0x1361,.size = 38 },
		{.start = 0x1362,.end = 0x138f,.size = 70 },
		{.start = 0x1390,.end = 0x1394,.size = 38 },
		{.start = 0x1395,.end = 0x1395,.size = 70 },
		{.start = 0x1396,.end = 0x1396,.size = 38 },
		{.start = 0x1397,.end = 0x1398,.size = 70 },
		{.start = 0x1399,.end = 0x1399,.size = 38 },
		{.start = 0x139a,.end = 0x139f,.size = 70 },
		{.start = 0x13a0,.end = 0x13f5,.size = 38 },
		{.start = 0x13f6,.end = 0x13f7,.size = 70 },
		{.start = 0x13f8,.end = 0x13fd,.size = 38 },
		{.start = 0x13fe,.end = 0x13ff,.size = 70 },
		{.start = 0x1400,.end = 0x1400,.size = 38 },
		{.start = 0x1401,.end = 0x141d,.size = 70 },
		{.start = 0x141e,.end = 0x142a,.size = 38 },
		{.start = 0x142b,.end = 0x1432,.size = 70 },
		{.start = 0x1433,.end = 0x1439,.size = 38 },
		{.start = 0x143a,.end = 0x1448,.size = 70 },
		{.start = 0x1449,.end = 0x144b,.size = 38 },
		{.start = 0x144c,.end = 0x1465,.size = 70 },
		{.start = 0x1466,.end = 0x1466,.size = 38 },
		{.start = 0x1467,.end = 0x146a,.size = 70 },
		{.start = 0x146b,.end = 0x14bf,.size = 38 },
		{.start = 0x14c0,.end = 0x14cf,.size = 70 },
		{.start = 0x14d0,.end = 0x14d2,.size = 38 },
		{.start = 0x14d3,.end = 0x14e9,.size = 70 },
		{.start = 0x14ea,.end = 0x150b,.size = 38 },
		{.start = 0x150c,.end = 0x1524,.size = 70 },
		{.start = 0x1525,.end = 0x1541,.size = 38 },
		{.start = 0x1542,.end = 0x1547,.size = 70 },
		{.start = 0x1548,.end = 0x1552,.size = 38 },
		{.start = 0x1553,.end = 0x155c,.size = 70 },
		{.start = 0x155d,.end = 0x155d,.size = 38 },
		{.start = 0x155e,.end = 0x1569,.size = 70 },
		{.start = 0x156a,.end = 0x156a,.size = 38 },
		{.start = 0x156b,.end = 0x157a,.size = 70 },
		{.start = 0x157b,.end = 0x157d,.size = 38 },
		{.start = 0x157e,.end = 0x1585,.size = 70 },
		{.start = 0x1586,.end = 0x1589,.size = 38 },
		{.start = 0x158a,.end = 0x1596,.size = 70 },
		{.start = 0x1597,.end = 0x159a,.size = 38 },
		{.start = 0x159b,.end = 0x159e,.size = 70 },
		{.start = 0x159f,.end = 0x159f,.size = 38 },
		{.start = 0x15a0,.end = 0x15a5,.size = 70 },
		{.start = 0x15a6,.end = 0x15b7,.size = 38 },
		{.start = 0x15b8,.end = 0x15ed,.size = 70 },
		{.start = 0x15ee,.end = 0x15ee,.size = 38 },
		{.start = 0x15ef,.end = 0x1600,.size = 70 },
		{.start = 0x1601,.end = 0x1603,.size = 38 },
		{.start = 0x1604,.end = 0x1645,.size = 70 },
		{.start = 0x1646,.end = 0x1647,.size = 38 },
		{.start = 0x1648,.end = 0x1659,.size = 70 },
		{.start = 0x165a,.end = 0x165a,.size = 38 },
		{.start = 0x165b,.end = 0x166c,.size = 70 },
		{.start = 0x166d,.end = 0x166e,.size = 38 },
		{.start = 0x166f,.end = 0x1676,.size = 70 },
		{.start = 0x1677,.end = 0x167f,.size = 38 },
		{.start = 0x1680,.end = 0x169f,.size = 70 },
		{.start = 0x16a0,.end = 0x16df,.size = 38 },
		{.start = 0x16e0,.end = 0x16e0,.size = 70 },
		{.start = 0x16e1,.end = 0x16e1,.size = 38 },
		{.start = 0x16e2,.end = 0x16e2,.size = 70 },
		{.start = 0x16e3,.end = 0x16f8,.size = 38 },
		{.start = 0x16f9,.end = 0x1761,.size = 70 },
		{.start = 0x1762,.end = 0x1762,.size = 38 },
		{.start = 0x1763,.end = 0x17ba,.size = 70 },
		{.start = 0x17bb,.end = 0x17bb,.size = 38 },
		{.start = 0x17bc,.end = 0x17ca,.size = 70 },
		{.start = 0x17cb,.end = 0x17cb,.size = 38 },
		{.start = 0x17cc,.end = 0x17d5,.size = 70 },
		{.start = 0x17d6,.end = 0x17d6,.size = 38 },
		{.start = 0x17d7,.end = 0x17db,.size = 70 },
		{.start = 0x17dc,.end = 0x17dc,.size = 38 },
		{.start = 0x17dd,.end = 0x17ef,.size = 70 },
		{.start = 0x17f0,.end = 0x17f9,.size = 38 },
		{.start = 0x17fa,.end = 0x18b3,.size = 70 },
		{.start = 0x18b4,.end = 0x18b6,.size = 38 },
		{.start = 0x18b7,.end = 0x18b7,.size = 70 },
		{.start = 0x18b8,.end = 0x18b8,.size = 38 },
		{.start = 0x18b9,.end = 0x18b9,.size = 70 },
		{.start = 0x18ba,.end = 0x18ba,.size = 38 },
		{.start = 0x18bb,.end = 0x18bd,.size = 70 },
		{.start = 0x18be,.end = 0x18bf,.size = 38 },
		{.start = 0x18c0,.end = 0x18c2,.size = 70 },
		{.start = 0x18c3,.end = 0x18c5,.size = 38 },
		{.start = 0x18c6,.end = 0x18d3,.size = 70 },
		{.start = 0x18d4,.end = 0x18df,.size = 38 },
		{.start = 0x18e0,.end = 0x18e8,.size = 70 },
		{.start = 0x18e9,.end = 0x18e9,.size = 38 },
		{.start = 0x18ea,.end = 0x18ea,.size = 70 },
		{.start = 0x18eb,.end = 0x18eb,.size = 38 },
		{.start = 0x18ec,.end = 0x18f2,.size = 70 },
		{.start = 0x18f3,.end = 0x18f5,.size = 38 },
		{.start = 0x18f6,.end = 0x194f,.size = 70 },
		{.start = 0x1950,.end = 0x196d,.size = 38 },
		{.start = 0x196e,.end = 0x196f,.size = 70 },
		{.start = 0x1970,.end = 0x1974,.size = 38 },
		{.start = 0x1975,.end = 0x1982,.size = 70 },
		{.start = 0x1983,.end = 0x1983,.size = 38 },
		{.start = 0x1984,.end = 0x198e,.size = 70 },
		{.start = 0x198f,.end = 0x198f,.size = 38 },
		{.start = 0x1990,.end = 0x1990,.size = 70 },
		{.start = 0x1991,.end = 0x1991,.size = 38 },
		{.start = 0x1992,.end = 0x1999,.size = 70 },
		{.start = 0x199a,.end = 0x199a,.size = 38 },
		{.start = 0x199b,.end = 0x199d,.size = 70 },
		{.start = 0x199e,.end = 0x199e,.size = 38 },
		{.start = 0x199f,.end = 0x19a0,.size = 70 },
		{.start = 0x19a1,.end = 0x19a1,.size = 38 },
		{.start = 0x19a2,.end = 0x19a2,.size = 70 },
		{.start = 0x19a3,.end = 0x19a4,.size = 38 },
		{.start = 0x19a5,.end = 0x19a6,.size = 70 },
		{.start = 0x19a7,.end = 0x19a7,.size = 38 },
		{.start = 0x19a8,.end = 0x19af,.size = 70 },
		{.start = 0x19b0,.end = 0x19b3,.size = 38 },
		{.start = 0x19b4,.end = 0x19b6,.size = 70 },
		{.start = 0x19b7,.end = 0x19ba,.size = 38 },
		{.start = 0x19bb,.end = 0x19c0,.size = 70 },
		{.start = 0x19c1,.end = 0x19c1,.size = 38 },
		{.start = 0x19c2,.end = 0x19c5,.size = 70 },
		{.start = 0x19c6,.end = 0x19c6,.size = 38 },
		{.start = 0x19c7,.end = 0x19c7,.size = 70 },
		{.start = 0x19c8,.end = 0x19c9,.size = 38 },
		{.start = 0x19ca,.end = 0x19cf,.size = 70 },
		{.start = 0x19d0,.end = 0x19d4,.size = 38 },
		{.start = 0x19d5,.end = 0x19d5,.size = 70 },
		{.start = 0x19d6,.end = 0x19d6,.size = 38 },
		{.start = 0x19d7,.end = 0x19d7,.size = 70 },
		{.start = 0x19d8,.end = 0x19d9,.size = 38 },
		{.start = 0x19da,.end = 0x1aaf,.size = 70 },
		{.start = 0x1ab0,.end = 0x1ac0,.size = 38 },
		{.start = 0x1ac1,.end = 0x1c4f,.size = 70 },
		{.start = 0x1c50,.end = 0x1c88,.size = 38 },
		{.start = 0x1c89,.end = 0x1c8f,.size = 70 },
		{.start = 0x1c90,.end = 0x1cba,.size = 38 },
		{.start = 0x1cbb,.end = 0x1cbc,.size = 70 },
		{.start = 0x1cbd,.end = 0x1cbf,.size = 38 },
		{.start = 0x1cc0,.end = 0x1cff,.size = 70 },
		{.start = 0x1d00,.end = 0x1d79,.size = 38 },
		{.start = 0x1d7a,.end = 0x1d7a,.size = 70 },
		{.start = 0x1d7b,.end = 0x1dcc,.size = 38 },
		{.start = 0x1dcd,.end = 0x1dcd,.size = 70 },
		{.start = 0x1dce,.end = 0x1df9,.size = 38 },
		{.start = 0x1dfa,.end = 0x1dfa,.size = 70 },
		{.start = 0x1dfb,.end = 0x1dfb,.size = 38 },
		{.start = 0x1dfc,.end = 0x1dfc,.size = 70 },
		{.start = 0x1dfd,.end = 0x1f15,.size = 38 },
		{.start = 0x1f16,.end = 0x1f17,.size = 70 },
		{.start = 0x1f18,.end = 0x1f1d,.size = 38 },
		{.start = 0x1f1e,.end = 0x1f1f,.size = 70 },
		{.start = 0x1f20,.end = 0x1f45,.size = 38 },
		{.start = 0x1f46,.end = 0x1f47,.size = 70 },
		{.start = 0x1f48,.end = 0x1f4d,.size = 38 },
		{.start = 0x1f4e,.end = 0x1f4f,.size = 70 },
		{.start = 0x1f50,.end = 0x1f57,.size = 38 },
		{.start = 0x1f58,.end = 0x1f58,.size = 70 },
		{.start = 0x1f59,.end = 0x1f59,.size = 38 },
		{.start = 0x1f5a,.end = 0x1f5a,.size = 70 },
		{.start = 0x1f5b,.end = 0x1f5b,.size = 38 },
		{.start = 0x1f5c,.end = 0x1f5c,.size = 70 },
		{.start = 0x1f5d,.end = 0x1f5d,.size = 38 },
		{.start = 0x1f5e,.end = 0x1f5e,.size = 70 },
		{.start = 0x1f5f,.end = 0x1f7d,.size = 38 },
		{.start = 0x1f7e,.end = 0x1f7f,.size = 70 },
		{.start = 0x1f80,.end = 0x1fb4,.size = 38 },
		{.start = 0x1fb5,.end = 0x1fb5,.size = 70 },
		{.start = 0x1fb6,.end = 0x1fc4,.size = 38 },
		{.start = 0x1fc5,.end = 0x1fc5,.size = 70 },
		{.start = 0x1fc6,.end = 0x1fd3,.size = 38 },
		{.start = 0x1fd4,.end = 0x1fd5,.size = 70 },
		{.start = 0x1fd6,.end = 0x1fdb,.size = 38 },
		{.start = 0x1fdc,.end = 0x1fdc,.size = 70 },
		{.start = 0x1fdd,.end = 0x1fef,.size = 38 },
		{.start = 0x1ff0,.end = 0x1ff1,.size = 70 },
		{.start = 0x1ff2,.end = 0x1ff4,.size = 38 },
		{.start = 0x1ff5,.end = 0x1ff5,.size = 70 },
		{.start = 0x1ff6,.end = 0x1ffe,.size = 38 },
		{.start = 0x1fff,.end = 0x1fff,.size = 70 },
		{.start = 0x2000,.end = 0x200a,.size = 38 },
		{.start = 0x200b,.end = 0x200f,.size = 70 },
		{.start = 0x2010,.end = 0x2027,.size = 38 },
		{.start = 0x2028,.end = 0x202e,.size = 70 },
		{.start = 0x202f,.end = 0x2056,.size = 38 },
		{.start = 0x2057,.end = 0x2057,.size = 70 },
		{.start = 0x2058,.end = 0x205f,.size = 38 },
		{.start = 0x2060,.end = 0x206f,.size = 70 },
		{.start = 0x2070,.end = 0x2071,.size = 38 },
		{.start = 0x2072,.end = 0x2073,.size = 70 },
		{.start = 0x2074,.end = 0x208e,.size = 38 },
		{.start = 0x208f,.end = 0x208f,.size = 70 },
		{.start = 0x2090,.end = 0x209c,.size = 38 },
		{.start = 0x209d,.end = 0x209f,.size = 70 },
		{.start = 0x20a0,.end = 0x20b8,.size = 38 },
		{.start = 0x20b9,.end = 0x20b9,.size = 70 },
		{.start = 0x20ba,.end = 0x20bf,.size = 38 },
		{.start = 0x20c0,.end = 0x20cf,.size = 70 },
		{.start = 0x20d0,.end = 0x20dc,.size = 38 },
		{.start = 0x20dd,.end = 0x20e0,.size = 70 },
		{.start = 0x20e1,.end = 0x20e1,.size = 38 },
		{.start = 0x20e2,.end = 0x20e4,.size = 70 },
		{.start = 0x20e5,.end = 0x20e6,.size = 38 },
		{.start = 0x20e7,.end = 0x20e7,.size = 70 },
		{.start = 0x20e8,.end = 0x20e9,.size = 38 },
		{.start = 0x20ea,.end = 0x20ea,.size = 70 },
		{.start = 0x20eb,.end = 0x20f0,.size = 38 },
		{.start = 0x20f1,.end = 0x20ff,.size = 70 },
		{.start = 0x2100,.end = 0x210d,.size = 38 },
		{.start = 0x210e,.end = 0x210f,.size = 70 },
		{.start = 0x2110,.end = 0x212d,.size = 38 },
		{.start = 0x212e,.end = 0x212e,.size = 70 },
		{.start = 0x212f,.end = 0x2139,.size = 38 },
		{.start = 0x213a,.end = 0x213d,.size = 70 },
		{.start = 0x213e,.end = 0x213e,.size = 38 },
		{.start = 0x213f,.end = 0x2140,.size = 70 },
		{.start = 0x2141,.end = 0x2144,.size = 38 },
		{.start = 0x2145,.end = 0x2149,.size = 70 },
		{.start = 0x214a,.end = 0x214b,.size = 38 },
		{.start = 0x214c,.end = 0x214c,.size = 70 },
		{.start = 0x214d,.end = 0x214e,.size = 38 },
		{.start = 0x214f,.end = 0x214f,.size = 70 },
		{.start = 0x2150,.end = 0x2181,.size = 38 },
		{.start = 0x2182,.end = 0x2182,.size = 70 },
		{.start = 0x2183,.end = 0x2187,.size = 38 },
		{.start = 0x2188,.end = 0x2188,.size = 70 },
		{.start = 0x2189,.end = 0x218b,.size = 38 },
		{.start = 0x218c,.end = 0x218f,.size = 70 },
		{.start = 0x2190,.end = 0x219b,.size = 38 },
		{.start = 0x219c,.end = 0x219d,.size = 70 },
		{.start = 0x219e,.end = 0x21f3,.size = 38 },
		{.start = 0x21f4,.end = 0x21f4,.size = 70 },
		{.start = 0x21f5,.end = 0x21f8,.size = 38 },
		{.start = 0x21f9,.end = 0x21fc,.size = 70 },
		{.start = 0x21fd,.end = 0x21fe,.size = 38 },
		{.start = 0x21ff,.end = 0x21ff,.size = 70 },
		{.start = 0x2200,.end = 0x22b5,.size = 38 },
		{.start = 0x22b6,.end = 0x22b8,.size = 70 },
		{.start = 0x22b9,.end = 0x22d7,.size = 38 },
		{.start = 0x22d8,.end = 0x22d9,.size = 70 },
		{.start = 0x22da,.end = 0x22f1,.size = 38 },
		{.start = 0x22f2,.end = 0x22f3,.size = 70 },
		{.start = 0x22f4,.end = 0x22f4,.size = 38 },
		{.start = 0x22f5,.end = 0x22f6,.size = 70 },
		{.start = 0x22f7,.end = 0x22f8,.size = 38 },
		{.start = 0x22f9,.end = 0x22fb,.size = 70 },
		{.start = 0x22fc,.end = 0x22fc,.size = 38 },
		{.start = 0x22fd,.end = 0x22fd,.size = 70 },
		{.start = 0x22fe,.end = 0x22fe,.size = 38 },
		{.start = 0x22ff,.end = 0x2300,.size = 70 },
		{.start = 0x2301,.end = 0x2315,.size = 38 },
		{.start = 0x2316,.end = 0x2316,.size = 70 },
		{.start = 0x2317,.end = 0x2328,.size = 38 },
		{.start = 0x2329,.end = 0x232a,.size = 70 },
		{.start = 0x232b,.end = 0x232b,.size = 38 },
		{.start = 0x232c,.end = 0x2335,.size = 70 },
		{.start = 0x2336,.end = 0x237a,.size = 38 },
		{.start = 0x237b,.end = 0x237e,.size = 70 },
		{.start = 0x237f,.end = 0x2380,.size = 38 },
		{.start = 0x2381,.end = 0x2394,.size = 70 },
		{.start = 0x2395,.end = 0x2396,.size = 38 },
		{.start = 0x2397,.end = 0x239a,.size = 70 },
		{.start = 0x239b,.end = 0x23b1,.size = 38 },
		{.start = 0x23b2,.end = 0x23b6,.size = 70 },
		{.start = 0x23b7,.end = 0x23bf,.size = 38 },
		{.start = 0x23c0,.end = 0x23ca,.size = 70 },
		{.start = 0x23cb,.end = 0x23cc,.size = 38 },
		{.start = 0x23cd,.end = 0x23ce,.size = 70 },
		{.start = 0x23cf,.end = 0x23d3,.size = 38 },
		{.start = 0x23d4,.end = 0x23d9,.size = 70 },
		{.start = 0x23da,.end = 0x23da,.size = 38 },
		{.start = 0x23db,.end = 0x23e7,.size = 70 },
		{.start = 0x23e8,.end = 0x23e8,.size = 38 },
		{.start = 0x23e9,.end = 0x2421,.size = 70 },
		{.start = 0x2422,.end = 0x2426,.size = 38 },
		{.start = 0x2427,.end = 0x243f,.size = 70 },
		{.start = 0x2440,.end = 0x244a,.size = 38 },
		{.start = 0x244b,.end = 0x24ff,.size = 70 },
		{.start = 0x2500,.end = 0x25ee,.size = 38 },
		{.start = 0x25ef,.end = 0x25ef,.size = 70 },
		{.start = 0x25f0,.end = 0x2602,.size = 38 },
		{.start = 0x2603,.end = 0x2603,.size = 70 },
		{.start = 0x2604,.end = 0x260f,.size = 38 },
		{.start = 0x2610,.end = 0x2612,.size = 70 },
		{.start = 0x2613,.end = 0x2614,.size = 38 },
		{.start = 0x2615,.end = 0x2619,.size = 70 },
		{.start = 0x261a,.end = 0x2621,.size = 38 },
		{.start = 0x2622,.end = 0x2624,.size = 70 },
		{.start = 0x2625,.end = 0x262a,.size = 38 },
		{.start = 0x262b,.end = 0x262c,.size = 70 },
		{.start = 0x262d,.end = 0x262e,.size = 38 },
		{.start = 0x262f,.end = 0x2637,.size = 70 },
		{.start = 0x2638,.end = 0x2671,.size = 38 },
		{.start = 0x2672,.end = 0x268f,.size = 70 },
		{.start = 0x2690,.end = 0x2691,.size = 38 },
		{.start = 0x2692,.end = 0x26a0,.size = 70 },
		{.start = 0x26a1,.end = 0x26a1,.size = 38 },
		{.start = 0x26a2,.end = 0x26a7,.size = 70 },
		{.start = 0x26a8,.end = 0x26a8,.size = 38 },
		{.start = 0x26a9,.end = 0x26a9,.size = 70 },
		{.start = 0x26aa,.end = 0x26ac,.size = 38 },
		{.start = 0x26ad,.end = 0x26b1,.size = 70 },
		{.start = 0x26b2,.end = 0x26b5,.size = 38 },
		{.start = 0x26b6,.end = 0x26b6,.size = 70 },
		{.start = 0x26b7,.end = 0x26bc,.size = 38 },
		{.start = 0x26bd,.end = 0x26e1,.size = 70 },
		{.start = 0x26e2,.end = 0x26e2,.size = 38 },
		{.start = 0x26e3,.end = 0x2767,.size = 70 },
		{.start = 0x2768,.end = 0x2775,.size = 38 },
		{.start = 0x2776,.end = 0x27af,.size = 70 },
		{.start = 0x27b0,.end = 0x27b0,.size = 38 },
		{.start = 0x27b1,.end = 0x27bf,.size = 70 },
		{.start = 0x27c0,.end = 0x27c0,.size = 38 },
		{.start = 0x27c1,.end = 0x27c1,.size = 70 },
		{.start = 0x27c2,.end = 0x27c2,.size = 38 },
		{.start = 0x27c3,.end = 0x27c4,.size = 70 },
		{.start = 0x27c5,.end = 0x27c7,.size = 38 },
		{.start = 0x27c8,.end = 0x27c9,.size = 70 },
		{.start = 0x27ca,.end = 0x27ca,.size = 38 },
		{.start = 0x27cb,.end = 0x27d0,.size = 70 },
		{.start = 0x27d1,.end = 0x27d1,.size = 38 },
		{.start = 0x27d2,.end = 0x27d2,.size = 70 },
		{.start = 0x27d3,.end = 0x27d4,.size = 38 },
		{.start = 0x27d5,.end = 0x27de,.size = 70 },
		{.start = 0x27df,.end = 0x27e0,.size = 38 },
		{.start = 0x27e1,.end = 0x27e5,.size = 70 },
		{.start = 0x27e6,.end = 0x27ef,.size = 38 },
		{.start = 0x27f0,.end = 0x27ff,.size = 70 },
		{.start = 0x2800,.end = 0x28ff,.size = 38 },
		{.start = 0x2900,.end = 0x2907,.size = 70 },
		{.start = 0x2908,.end = 0x2909,.size = 38 },
		{.start = 0x290a,.end = 0x2911,.size = 70 },
		{.start = 0x2912,.end = 0x2913,.size = 38 },
		{.start = 0x2914,.end = 0x2937,.size = 70 },
		{.start = 0x2938,.end = 0x2939,.size = 38 },
		{.start = 0x293a,.end = 0x2948,.size = 70 },
		{.start = 0x2949,.end = 0x2949,.size = 38 },
		{.start = 0x294a,.end = 0x294b,.size = 70 },
		{.start = 0x294c,.end = 0x294d,.size = 38 },
		{.start = 0x294e,.end = 0x294e,.size = 70 },
		{.start = 0x294f,.end = 0x294f,.size = 38 },
		{.start = 0x2950,.end = 0x2950,.size = 70 },
		{.start = 0x2951,.end = 0x2951,.size = 38 },
		{.start = 0x2952,.end = 0x2953,.size = 70 },
		{.start = 0x2954,.end = 0x2955,.size = 38 },
		{.start = 0x2956,.end = 0x2957,.size = 70 },
		{.start = 0x2958,.end = 0x2959,.size = 38 },
		{.start = 0x295a,.end = 0x295b,.size = 70 },
		{.start = 0x295c,.end = 0x295d,.size = 38 },
		{.start = 0x295e,.end = 0x295f,.size = 70 },
		{.start = 0x2960,.end = 0x2961,.size = 38 },
		{.start = 0x2962,.end = 0x297b,.size = 70 },
		{.start = 0x297c,.end = 0x297d,.size = 38 },
		{.start = 0x297e,.end = 0x297f,.size = 70 },
		{.start = 0x2980,.end = 0x2992,.size = 38 },
		{.start = 0x2993,.end = 0x2996,.size = 70 },
		{.start = 0x2997,.end = 0x299d,.size = 38 },
		{.start = 0x299e,.end = 0x299e,.size = 70 },
		{.start = 0x299f,.end = 0x29a7,.size = 38 },
		{.start = 0x29a8,.end = 0x29d0,.size = 70 },
		{.start = 0x29d1,.end = 0x29d9,.size = 38 },
		{.start = 0x29da,.end = 0x29db,.size = 70 },
		{.start = 0x29dc,.end = 0x29de,.size = 38 },
		{.start = 0x29df,.end = 0x29e0,.size = 70 },
		{.start = 0x29e1,.end = 0x29e1,.size = 38 },
		{.start = 0x29e2,.end = 0x29ea,.size = 70 },
		{.start = 0x29eb,.end = 0x29eb,.size = 38 },
		{.start = 0x29ec,.end = 0x29ed,.size = 70 },
		{.start = 0x29ee,.end = 0x29f3,.size = 38 },
		{.start = 0x29f4,.end = 0x29f4,.size = 70 },
		{.start = 0x29f5,.end = 0x29fd,.size = 38 },
		{.start = 0x29fe,.end = 0x2a0a,.size = 70 },
		{.start = 0x2a0b,.end = 0x2a0b,.size = 38 },
		{.start = 0x2a0c,.end = 0x2a0c,.size = 70 },
		{.start = 0x2a0d,.end = 0x2a1c,.size = 38 },
		{.start = 0x2a1d,.end = 0x2a1d,.size = 70 },
		{.start = 0x2a1e,.end = 0x2a1f,.size = 38 },
		{.start = 0x2a20,.end = 0x2a20,.size = 70 },
		{.start = 0x2a21,.end = 0x2a2c,.size = 38 },
		{.start = 0x2a2d,.end = 0x2a2e,.size = 70 },
		{.start = 0x2a2f,.end = 0x2a32,.size = 38 },
		{.start = 0x2a33,.end = 0x2a3b,.size = 70 },
		{.start = 0x2a3c,.end = 0x2a4d,.size = 38 },
		{.start = 0x2a4e,.end = 0x2a65,.size = 70 },
		{.start = 0x2a66,.end = 0x2a67,.size = 38 },
		{.start = 0x2a68,.end = 0x2a69,.size = 70 },
		{.start = 0x2a6a,.end = 0x2a73,.size = 38 },
		{.start = 0x2a74,.end = 0x2a76,.size = 70 },
		{.start = 0x2a77,.end = 0x2a77,.size = 38 },
		{.start = 0x2a78,.end = 0x2a8a,.size = 70 },
		{.start = 0x2a8b,.end = 0x2a8c,.size = 38 },
		{.start = 0x2a8d,.end = 0x2a8e,.size = 70 },
		{.start = 0x2a8f,.end = 0x2a94,.size = 38 },
		{.start = 0x2a95,.end = 0x2abe,.size = 70 },
		{.start = 0x2abf,.end = 0x2acc,.size = 38 },
		{.start = 0x2acd,.end = 0x2ad2,.size = 70 },
		{.start = 0x2ad3,.end = 0x2ad6,.size = 38 },
		{.start = 0x2ad7,.end = 0x2add,.size = 70 },
		{.start = 0x2ade,.end = 0x2ade,.size = 38 },
		{.start = 0x2adf,.end = 0x2aed,.size = 70 },
		{.start = 0x2aee,.end = 0x2af2,.size = 38 },
		{.start = 0x2af3,.end = 0x2af5,.size = 70 },
		{.start = 0x2af6,.end = 0x2af6,.size = 38 },
		{.start = 0x2af7,.end = 0x2afd,.size = 70 },
		{.start = 0x2afe,.end = 0x2aff,.size = 38 },
		{.start = 0x2b00,.end = 0x2b05,.size = 70 },
		{.start = 0x2b06,.end = 0x2b07,.size = 38 },
		{.start = 0x2b08,.end = 0x2b0c,.size = 70 },
		{.start = 0x2b0d,.end = 0x2b0d,.size = 38 },
		{.start = 0x2b0e,.end = 0x2b1c,.size = 70 },
		{.start = 0x2b1d,.end = 0x2b1e,.size = 38 },
		{.start = 0x2b1f,.end = 0x2b24,.size = 70 },
		{.start = 0x2b25,.end = 0x2b2b,.size = 38 },
		{.start = 0x2b2c,.end = 0x2b2d,.size = 70 },
		{.start = 0x2b2e,.end = 0x2b2f,.size = 38 },
		{.start = 0x2b30,.end = 0x2b30,.size = 70 },
		{.start = 0x2b31,.end = 0x2b31,.size = 38 },
		{.start = 0x2b32,.end = 0x2b4d,.size = 70 },
		{.start = 0x2b4e,.end = 0x2b4f,.size = 38 },
		{.start = 0x2b50,.end = 0x2bc8,.size = 70 },
		{.start = 0x2bc9,.end = 0x2bc9,.size = 38 },
		{.start = 0x2bca,.end = 0x2bfe,.size = 70 },
		{.start = 0x2bff,.end = 0x2c0e,.size = 38 },
		{.start = 0x2c0f,.end = 0x2c0f,.size = 70 },
		{.start = 0x2c10,.end = 0x2c1e,.size = 38 },
		{.start = 0x2c1f,.end = 0x2c1f,.size = 70 },
		{.start = 0x2c20,.end = 0x2c26,.size = 38 },
		{.start = 0x2c27,.end = 0x2c29,.size = 70 },
		{.start = 0x2c2a,.end = 0x2c2e,.size = 38 },
		{.start = 0x2c2f,.end = 0x2c2f,.size = 70 },
		{.start = 0x2c30,.end = 0x2c3e,.size = 38 },
		{.start = 0x2c3f,.end = 0x2c3f,.size = 70 },
		{.start = 0x2c40,.end = 0x2c4e,.size = 38 },
		{.start = 0x2c4f,.end = 0x2c4f,.size = 70 },
		{.start = 0x2c50,.end = 0x2c56,.size = 38 },
		{.start = 0x2c57,.end = 0x2c59,.size = 70 },
		{.start = 0x2c5a,.end = 0x2c5e,.size = 38 },
		{.start = 0x2c5f,.end = 0x2c5f,.size = 70 },
		{.start = 0x2c60,.end = 0x2ce6,.size = 38 },
		{.start = 0x2ce7,.end = 0x2ce7,.size = 70 },
		{.start = 0x2ce8,.end = 0x2ce9,.size = 38 },
		{.start = 0x2cea,.end = 0x2cea,.size = 70 },
		{.start = 0x2ceb,.end = 0x2cee,.size = 38 },
		{.start = 0x2cef,.end = 0x2cef,.size = 70 },
		{.start = 0x2cf0,.end = 0x2cf3,.size = 38 },
		{.start = 0x2cf4,.end = 0x2cf8,.size = 70 },
		{.start = 0x2cf9,.end = 0x2d04,.size = 38 },
		{.start = 0x2d05,.end = 0x2d05,.size = 70 },
		{.start = 0x2d06,.end = 0x2d06,.size = 38 },
		{.start = 0x2d07,.end = 0x2d07,.size = 70 },
		{.start = 0x2d08,.end = 0x2d09,.size = 38 },
		{.start = 0x2d0a,.end = 0x2d0a,.size = 70 },
		{.start = 0x2d0b,.end = 0x2d0c,.size = 38 },
		{.start = 0x2d0d,.end = 0x2d0d,.size = 70 },
		{.start = 0x2d0e,.end = 0x2d0f,.size = 38 },
		{.start = 0x2d10,.end = 0x2d10,.size = 70 },
		{.start = 0x2d11,.end = 0x2d12,.size = 38 },
		{.start = 0x2d13,.end = 0x2d14,.size = 70 },
		{.start = 0x2d15,.end = 0x2d1a,.size = 38 },
		{.start = 0x2d1b,.end = 0x2d1c,.size = 70 },
		{.start = 0x2d1d,.end = 0x2d1f,.size = 38 },
		{.start = 0x2d20,.end = 0x2d20,.size = 70 },
		{.start = 0x2d21,.end = 0x2d24,.size = 38 },
		{.start = 0x2d25,.end = 0x2d26,.size = 70 },
		{.start = 0x2d27,.end = 0x2d27,.size = 38 },
		{.start = 0x2d28,.end = 0x2d2c,.size = 70 },
		{.start = 0x2d2d,.end = 0x2d2d,.size = 38 },
		{.start = 0x2d2e,.end = 0x2d2f,.size = 70 },
		{.start = 0x2d30,.end = 0x2d47,.size = 38 },
		{.start = 0x2d48,.end = 0x2d48,.size = 70 },
		{.start = 0x2d49,.end = 0x2d67,.size = 38 },
		{.start = 0x2d68,.end = 0x2d6e,.size = 70 },
		{.start = 0x2d6f,.end = 0x2d70,.size = 38 },
		{.start = 0x2d71,.end = 0x2d7e,.size = 70 },
		{.start = 0x2d7f,.end = 0x2d7f,.size = 38 },
		{.start = 0x2d80,.end = 0x2ddf,.size = 70 },
		{.start = 0x2de0,.end = 0x2e0d,.size = 38 },
		{.start = 0x2e0e,.end = 0x2e11,.size = 70 },
		{.start = 0x2e12,.end = 0x2e12,.size = 38 },
		{.start = 0x2e13,.end = 0x2e15,.size = 70 },
		{.start = 0x2e16,.end = 0x2e39,.size = 38 },
		{.start = 0x2e3a,.end = 0x2e3b,.size = 70 },
		{.start = 0x2e3c,.end = 0x2e42,.size = 38 },
		{.start = 0x2e43,.end = 0x2e43,.size = 70 },
		{.start = 0x2e44,.end = 0x2e4f,.size = 38 },
		{.start = 0x2e50,.end = 0x2e51,.size = 70 },
		{.start = 0x2e52,.end = 0x2e52,.size = 38 },
		{.start = 0x2e53,.end = 0x303e,.size = 70 },
		{.start = 0x303f,.end = 0x303f,.size = 38 },
		{.start = 0x3040,.end = 0xa4cf,.size = 70 },
		{.start = 0xa4d0,.end = 0xa4ff,.size = 38 },
		{.start = 0xa500,.end = 0xa63f,.size = 70 },
		{.start = 0xa640,.end = 0xa64b,.size = 38 },
		{.start = 0xa64c,.end = 0xa64d,.size = 70 },
		{.start = 0xa64e,.end = 0xa65d,.size = 38 },
		{.start = 0xa65e,.end = 0xa65e,.size = 70 },
		{.start = 0xa65f,.end = 0xa661,.size = 38 },
		{.start = 0xa662,.end = 0xa667,.size = 70 },
		{.start = 0xa668,.end = 0xa66b,.size = 38 },
		{.start = 0xa66c,.end = 0xa66e,.size = 70 },
		{.start = 0xa66f,.end = 0xa66f,.size = 38 },
		{.start = 0xa670,.end = 0xa670,.size = 70 },
		{.start = 0xa671,.end = 0xa671,.size = 38 },
		{.start = 0xa672,.end = 0xa672,.size = 70 },
		{.start = 0xa673,.end = 0xa683,.size = 38 },
		{.start = 0xa684,.end = 0xa685,.size = 70 },
		{.start = 0xa686,.end = 0xa697,.size = 38 },
		{.start = 0xa698,.end = 0xa699,.size = 70 },
		{.start = 0xa69a,.end = 0xa6f7,.size = 38 },
		{.start = 0xa6f8,.end = 0xa6ff,.size = 70 },
		{.start = 0xa700,.end = 0xa727,.size = 38 },
		{.start = 0xa728,.end = 0xa729,.size = 70 },
		{.start = 0xa72a,.end = 0xa731,.size = 38 },
		{.start = 0xa732,.end = 0xa73d,.size = 70 },
		{.start = 0xa73e,.end = 0xa74d,.size = 38 },
		{.start = 0xa74e,.end = 0xa74f,.size = 70 },
		{.start = 0xa750,.end = 0xa757,.size = 38 },
		{.start = 0xa758,.end = 0xa759,.size = 70 },
		{.start = 0xa75a,.end = 0xa770,.size = 38 },
		{.start = 0xa771,.end = 0xa777,.size = 70 },
		{.start = 0xa778,.end = 0xa7bf,.size = 38 },
		{.start = 0xa7c0,.end = 0xa7c3,.size = 70 },
		{.start = 0xa7c4,.end = 0xa7ca,.size = 38 },
		{.start = 0xa7cb,.end = 0xa7f4,.size = 70 },
		{.start = 0xa7f5,.end = 0xa7fe,.size = 38 },
		{.start = 0xa7ff,.end = 0xa827,.size = 70 },
		{.start = 0xa828,.end = 0xa829,.size = 38 },
		{.start = 0xa82a,.end = 0xab2f,.size = 70 },
		{.start = 0xab30,.end = 0xab65,.size = 38 },
		{.start = 0xab66,.end = 0xab67,.size = 70 },
		{.start = 0xab68,.end = 0xab6b,.size = 38 },
		{.start = 0xab6c,.end = 0xab6f,.size = 70 },
		{.start = 0xab70,.end = 0xabbf,.size = 38 },
		{.start = 0xabc0,.end = 0xd7ff,.size = 70 },
		{.start = 0xf900,.end = 0xfaff,.size = 70 },
		{.start = 0xfb00,.end = 0xfb06,.size = 38 },
		{.start = 0xfb07,.end = 0xfb12,.size = 70 },
		{.start = 0xfb13,.end = 0xfb17,.size = 38 },
		{.start = 0xfb18,.end = 0xfb1c,.size = 70 },
		{.start = 0xfb1d,.end = 0xfb20,.size = 38 },
		{.start = 0xfb21,.end = 0xfb28,.size = 70 },
		{.start = 0xfb29,.end = 0xfb36,.size = 38 },
		{.start = 0xfb37,.end = 0xfb37,.size = 70 },
		{.start = 0xfb38,.end = 0xfb3c,.size = 38 },
		{.start = 0xfb3d,.end = 0xfb3d,.size = 70 },
		{.start = 0xfb3e,.end = 0xfb3e,.size = 38 },
		{.start = 0xfb3f,.end = 0xfb3f,.size = 70 },
		{.start = 0xfb40,.end = 0xfb41,.size = 38 },
		{.start = 0xfb42,.end = 0xfb42,.size = 70 },
		{.start = 0xfb43,.end = 0xfb44,.size = 38 },
		{.start = 0xfb45,.end = 0xfb45,.size = 70 },
		{.start = 0xfb46,.end = 0xfbc1,.size = 38 },
		{.start = 0xfbc2,.end = 0xfbd2,.size = 70 },
		{.start = 0xfbd3,.end = 0xfc1e,.size = 38 },
		{.start = 0xfc1f,.end = 0xfc1f,.size = 70 },
		{.start = 0xfc20,.end = 0xfc20,.size = 38 },
		{.start = 0xfc21,.end = 0xfc21,.size = 70 },
		{.start = 0xfc22,.end = 0xfc24,.size = 38 },
		{.start = 0xfc25,.end = 0xfc25,.size = 70 },
		{.start = 0xfc26,.end = 0xfc3c,.size = 38 },
		{.start = 0xfc3d,.end = 0xfc3e,.size = 70 },
		{.start = 0xfc3f,.end = 0xfcac,.size = 38 },
		{.start = 0xfcad,.end = 0xfcaf,.size = 70 },
		{.start = 0xfcb0,.end = 0xfcb0,.size = 38 },
		{.start = 0xfcb1,.end = 0xfcb7,.size = 70 },
		{.start = 0xfcb8,.end = 0xfce6,.size = 38 },
		{.start = 0xfce7,.end = 0xfcea,.size = 70 },
		{.start = 0xfceb,.end = 0xfcfa,.size = 38 },
		{.start = 0xfcfb,.end = 0xfcfe,.size = 70 },
		{.start = 0xfcff,.end = 0xfd16,.size = 38 },
		{.start = 0xfd17,.end = 0xfd20,.size = 70 },
		{.start = 0xfd21,.end = 0xfd2c,.size = 38 },
		{.start = 0xfd2d,.end = 0xfd32,.size = 70 },
		{.start = 0xfd33,.end = 0xfd33,.size = 38 },
		{.start = 0xfd34,.end = 0xfd39,.size = 70 },
		{.start = 0xfd3a,.end = 0xfd3d,.size = 38 },
		{.start = 0xfd3e,.end = 0xfe1f,.size = 70 },
		{.start = 0xfe20,.end = 0xfe2f,.size = 38 },
		{.start = 0xfe30,.end = 0xfe6f,.size = 70 },
		{.start = 0xfe70,.end = 0xfe74,.size = 38 },
		{.start = 0xfe75,.end = 0xfe75,.size = 70 },
		{.start = 0xfe76,.end = 0xfefc,.size = 38 },
		{.start = 0xfefd,.end = 0xff60,.size = 70 },
		{.start = 0xff61,.end = 0xffbe,.size = 38 },
		{.start = 0xffbf,.end = 0xffc1,.size = 70 },
		{.start = 0xffc2,.end = 0xffc7,.size = 38 },
		{.start = 0xffc8,.end = 0xffc9,.size = 70 },
		{.start = 0xffca,.end = 0xffcf,.size = 38 },
		{.start = 0xffd0,.end = 0xffd1,.size = 70 },
		{.start = 0xffd2,.end = 0xffd7,.size = 38 },
		{.start = 0xffd8,.end = 0xffd9,.size = 70 },
		{.start = 0xffda,.end = 0xffdc,.size = 38 },
		{.start = 0xffdd,.end = 0xffe7,.size = 70 },
		{.start = 0xffe8,.end = 0xffee,.size = 38 },
		{.start = 0xffef,.end = 0xfffc,.size = 70 },
		{.start = 0xfffd,.end = 0xfffd,.size = 38 } };

static int is_number(uint8_t ch, uint8_t base) {
	if (base < 2) {
		return -1;
	} else if (base == 2) {
		return ch == '1' || ch == '0';
	} else if (base > 2 && base <= 36) {
		if (base <= 10) {
			return ch >= '0' && ch <= '0' + base - 1;
		} else {
			return (ch >= '0' && ch <= '9')
					|| (ch >= 'A' && ch <= 'A' + base - 11)
					|| (ch >= 'a' && ch <= 'a' + base - 11);
		}
	} else {
		return -1;
	}
}

static int parse_int(const char *str, size_t str_len, uint8_t base) {
	int symbol = 1;
	int number = 0;
	int i = 0;

	if (str == NULL)
		return 0;

	size_t size = strnlen(str, str_len);
	if (size > 0) {
		symbol = str[i] == '-' ? (i++, -1) : str[i] == '+' ? (i++, 1) : 1;
		if ((i == 0 && size >= 2) || (i == 1 && size >= 3)) {
			if (base == 2 && str[i] == '0' && str[i + 1] == 'b') {
				i += 2;
			} else if (base == 8 && str[i] == '0' && str[i + 1] == 'o') {
				i += 2;
			} else if (base == 16 && str[i] == '0' && str[i + 1] == 'x') {
				i += 2;
			}
		}
		while (i < size && is_number(str[i], base) > 0) {
			char ch = str[i] >= 'a' ? str[i] - 32 : str[i];

			if (ch >= 'A') {
				number = number * base + ch - 'A' + 10;
			} else {
				number = number * base + ch - '0';
			}
			i++;
		}
	}
	return symbol * number;
}

static lv_fs_res_t lv_fs_eof(lv_fs_file_t *file_p, _Bool *result) {
	uint32_t fs_size;
	uint32_t fs_pos;
	lv_fs_res_t res = lv_fs_size(file_p, &fs_size);
	if (res == LV_FS_RES_OK) {
		res = lv_fs_tell(file_p, &fs_pos);
		if (res == LV_FS_RES_OK) {
			*result = fs_pos == (fs_size - 1);
		}
	}
	return res;
}

static char* lv_fs_read_line(lv_fs_file_t *file_p, size_t *buffer_size,
		size_t *count) {
	char *buffer = (char*) lv_mem_alloc(10 * sizeof(char));
	*count = 0;
	if (buffer) {
		*buffer_size = 10;
		_Bool eof = false;
		while (lv_fs_eof(file_p, &eof) == LV_FS_RES_OK && !eof) {
			uint32_t br;
			if ((*count) >= (*buffer_size) - 1) {
				char *ptr = NULL;
				ptr = (char*) lv_mem_realloc(buffer,
						(*buffer_size) + 10 * sizeof(char));
				if (ptr == NULL) {
					break;
				} else {
					buffer = ptr;
					(*buffer_size) += 10;
				}
			}
			if (lv_fs_read(file_p, buffer + (*count), 1, &br) == LV_FS_RES_OK) {
				if (buffer[*count] == '\n') {
					break;
				}
				(*count) += br;
			} else {
				break;
			}

		}
	}
	buffer[(*count)] = '\0';
	return buffer;
}

static uint32_t get_font_in_file_pos(uint32_t unicode_letter) {
	uint32_t pos = 0;
	for (int i = 0; i < sizeof(_font_index) / sizeof(_UnifontIndex); i++) {
		_UnifontIndex index = _font_index[i];
		if (unicode_letter >= index.start && unicode_letter <= index.end) {
			pos += (unicode_letter - index.start) * index.size;
			break;
		} else {
			pos += (index.end - index.start + 1) * index.size;
		}
	}
	return pos;
}

static void decode_font_data(uint8_t *buffer, char *font_data,
		size_t font_data_size) {
	assert((font_data_size % 2) == 0);
	char *ptr = font_data;
	uint8_t *buffer_ptr = buffer;
	while ((ptr - font_data) < font_data_size) {
		*buffer_ptr = (uint8_t) parse_int(ptr, 2, 16);
		ptr += 2;
		buffer_ptr++;
	}
}

static const uint8_t* unifont_get_glyph_bitmap(const lv_font_t *font,
		uint32_t unicode_letter) {
	if (!font_file_opened) {
		if (lv_fs_open(&font_file, FONT_FILE, LV_FS_MODE_RD) == LV_FS_RES_OK) {
			font_file_opened = true;
		}
	}
	if (lv_fs_seek(&font_file, get_font_in_file_pos(unicode_letter))
			== LV_FS_RES_OK) {
		size_t buffer_size = 0;
		size_t count = 0;
		char *font_data_ptr = NULL;
		char *buffer = NULL;
		while ((buffer = lv_fs_read_line(&font_file, &buffer_size, &count))
				!= NULL) {
			if (count > 0) {
				font_data_ptr = strchr(buffer, ':'); //找到分割点
				if (font_data_ptr != NULL) {
					uint32_t unicode = parse_int(buffer, font_data_ptr - buffer,
							16);
					if (unicode == unicode_letter) {
						font_data_ptr += 1; //后移动一位
						decode_font_data(font_glyph, font_data_ptr,
								count - (font_data_ptr - buffer));
						lv_mem_free(buffer);
						return font_glyph;
					} else {
						lv_mem_free(buffer);
					}
				}
			} else {
				lv_mem_free(buffer);
				break;
			}
		}
	}

	return NULL;
}

static _Bool unifont_get_glyph_dsc(const lv_font_t *font,
		lv_font_glyph_dsc_t *dsc_out, uint32_t unicode_letter,
		uint32_t unicode_letter_next) {
	if (!font_file_opened) {
		if (lv_fs_open(&font_file, FONT_FILE, LV_FS_MODE_RD) == LV_FS_RES_OK) {
			font_file_opened = true;
		}
	}

	if (lv_fs_seek(&font_file, get_font_in_file_pos(unicode_letter))
			== LV_FS_RES_OK) {
		size_t buffer_size = 0;
		size_t count = 0;
		char *font_data_ptr = NULL;
		char *buffer = NULL;
		while ((buffer = lv_fs_read_line(&font_file, &buffer_size, &count))
				!= NULL) {
			if (count > 0) {
				font_data_ptr = strchr(buffer, ':'); //找到分割点
				if (font_data_ptr != NULL) {
					uint32_t unicode = parse_int(buffer, font_data_ptr - buffer,
							16);
					if (unicode == unicode_letter) {
						font_data_ptr += 1; //后移动一位
						dsc_out->bpp = 1;
						dsc_out->ofs_x = 0;
						dsc_out->ofs_y = 0;
						dsc_out->box_h = 16;
						if (count - (font_data_ptr - buffer) == 32) {
							dsc_out->box_w = 8;
							dsc_out->adv_w = 8;
						} else {
							dsc_out->box_w = 16;
							dsc_out->adv_w = 16;
						}
						lv_mem_free(buffer);
						return true;
					} else {
						lv_mem_free(buffer);
						break;
					}
				}
			} else {
				lv_mem_free(buffer);
				break;
			}
		}

	}
	return false;
}

lv_font_t lv_font_unifont_16 = { .get_glyph_dsc = unifont_get_glyph_dsc,
		.base_line = 1, .line_height = 18, .get_glyph_bitmap =
				unifont_get_glyph_bitmap,
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
		.subpx = LV_FONT_SUBPX_NONE,
#endif
		.dsc = NULL

};

void lv_font_unifont_16_close(){
	if(font_file_opened){
		lv_fs_close(&font_file);
	}
}
