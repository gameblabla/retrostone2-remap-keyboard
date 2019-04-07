/****************************************************************************************/
/* SUNXI GPIO library interface                                                         */
/****************************************************************************************/

#ifndef SUNXI_GPIO_H_
#define SUNXI_GPIO_H_


/****************************************************************************************/
/* Includes                                                                             */
/****************************************************************************************/

#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>


/****************************************************************************************/
/* Definitions                                                                          */
/****************************************************************************************/

/* SUNXI GPIO pin function configuration */
#define SUNXI_GPIO_INPUT                        0
#define SUNXI_GPIO_OUTPUT                       1
#define SUNXI_GPIO_PER                          2

/* SUNXI GPIO macro */
#define SUNXI_GPIO_PIN(port, pin)               ((port - 'A') << 5) + pin

/* SUNXI GPIOs */
#define SUNXI_GPIO_PIN_PA0                      0
#define SUNXI_GPIO_PIN_PA1                      1
#define SUNXI_GPIO_PIN_PA2                      2
#define SUNXI_GPIO_PIN_PA3                      3
#define SUNXI_GPIO_PIN_PA4                      4
#define SUNXI_GPIO_PIN_PA5                      5
#define SUNXI_GPIO_PIN_PA6                      6
#define SUNXI_GPIO_PIN_PA7                      7
#define SUNXI_GPIO_PIN_PA8                      8
#define SUNXI_GPIO_PIN_PA9                      9
#define SUNXI_GPIO_PIN_PA10                     10
#define SUNXI_GPIO_PIN_PA11                     11
#define SUNXI_GPIO_PIN_PA12                     12
#define SUNXI_GPIO_PIN_PA13                     13
#define SUNXI_GPIO_PIN_PA14                     14
#define SUNXI_GPIO_PIN_PA15                     15
#define SUNXI_GPIO_PIN_PA16                     16
#define SUNXI_GPIO_PIN_PA17                     17
#define SUNXI_GPIO_PIN_PA18                     18
#define SUNXI_GPIO_PIN_PA19                     19
#define SUNXI_GPIO_PIN_PA20                     20
#define SUNXI_GPIO_PIN_PA21                     21
#define SUNXI_GPIO_PIN_PA22                     22
#define SUNXI_GPIO_PIN_PA23                     23
#define SUNXI_GPIO_PIN_PA24                     24
#define SUNXI_GPIO_PIN_PA25                     25
#define SUNXI_GPIO_PIN_PA26                     26
#define SUNXI_GPIO_PIN_PA27                     27
#define SUNXI_GPIO_PIN_PA28                     28
#define SUNXI_GPIO_PIN_PA29                     29
#define SUNXI_GPIO_PIN_PA30                     30
#define SUNXI_GPIO_PIN_PA31                     31
#define SUNXI_GPIO_PIN_PB0                      32
#define SUNXI_GPIO_PIN_PB1                      33
#define SUNXI_GPIO_PIN_PB2                      34
#define SUNXI_GPIO_PIN_PB3                      35
#define SUNXI_GPIO_PIN_PB4                      36
#define SUNXI_GPIO_PIN_PB5                      37
#define SUNXI_GPIO_PIN_PB6                      38
#define SUNXI_GPIO_PIN_PB7                      39
#define SUNXI_GPIO_PIN_PB8                      40
#define SUNXI_GPIO_PIN_PB9                      41
#define SUNXI_GPIO_PIN_PB10                     42
#define SUNXI_GPIO_PIN_PB11                     43
#define SUNXI_GPIO_PIN_PB12                     44
#define SUNXI_GPIO_PIN_PB13                     45
#define SUNXI_GPIO_PIN_PB14                     46
#define SUNXI_GPIO_PIN_PB15                     47
#define SUNXI_GPIO_PIN_PB16                     48
#define SUNXI_GPIO_PIN_PB17                     49
#define SUNXI_GPIO_PIN_PB18                     50
#define SUNXI_GPIO_PIN_PB19                     51
#define SUNXI_GPIO_PIN_PB20                     52
#define SUNXI_GPIO_PIN_PB21                     53
#define SUNXI_GPIO_PIN_PB22                     54
#define SUNXI_GPIO_PIN_PB23                     55
#define SUNXI_GPIO_PIN_PB24                     56
#define SUNXI_GPIO_PIN_PB25                     57
#define SUNXI_GPIO_PIN_PB26                     58
#define SUNXI_GPIO_PIN_PB27                     59
#define SUNXI_GPIO_PIN_PB28                     60
#define SUNXI_GPIO_PIN_PB29                     61
#define SUNXI_GPIO_PIN_PB30                     62
#define SUNXI_GPIO_PIN_PB31                     63
#define SUNXI_GPIO_PIN_PC0                      64
#define SUNXI_GPIO_PIN_PC1                      65
#define SUNXI_GPIO_PIN_PC2                      66
#define SUNXI_GPIO_PIN_PC3                      67
#define SUNXI_GPIO_PIN_PC4                      68
#define SUNXI_GPIO_PIN_PC5                      69
#define SUNXI_GPIO_PIN_PC6                      70
#define SUNXI_GPIO_PIN_PC7                      71
#define SUNXI_GPIO_PIN_PC8                      72
#define SUNXI_GPIO_PIN_PC9                      73
#define SUNXI_GPIO_PIN_PC10                     74
#define SUNXI_GPIO_PIN_PC11                     75
#define SUNXI_GPIO_PIN_PC12                     76
#define SUNXI_GPIO_PIN_PC13                     77
#define SUNXI_GPIO_PIN_PC14                     78
#define SUNXI_GPIO_PIN_PC15                     79
#define SUNXI_GPIO_PIN_PC16                     80
#define SUNXI_GPIO_PIN_PC17                     81
#define SUNXI_GPIO_PIN_PC18                     82
#define SUNXI_GPIO_PIN_PC19                     83
#define SUNXI_GPIO_PIN_PC20                     84
#define SUNXI_GPIO_PIN_PC21                     85
#define SUNXI_GPIO_PIN_PC22                     86
#define SUNXI_GPIO_PIN_PC23                     87
#define SUNXI_GPIO_PIN_PC24                     88
#define SUNXI_GPIO_PIN_PC25                     89
#define SUNXI_GPIO_PIN_PC26                     90
#define SUNXI_GPIO_PIN_PC27                     91
#define SUNXI_GPIO_PIN_PC28                     92
#define SUNXI_GPIO_PIN_PC29                     93
#define SUNXI_GPIO_PIN_PC30                     94
#define SUNXI_GPIO_PIN_PC31                     95
#define SUNXI_GPIO_PIN_PD0                      96
#define SUNXI_GPIO_PIN_PD1                      97
#define SUNXI_GPIO_PIN_PD2                      98
#define SUNXI_GPIO_PIN_PD3                      99
#define SUNXI_GPIO_PIN_PD4                      100
#define SUNXI_GPIO_PIN_PD5                      101
#define SUNXI_GPIO_PIN_PD6                      102
#define SUNXI_GPIO_PIN_PD7                      103
#define SUNXI_GPIO_PIN_PD8                      104
#define SUNXI_GPIO_PIN_PD9                      105
#define SUNXI_GPIO_PIN_PD10                     106
#define SUNXI_GPIO_PIN_PD11                     107
#define SUNXI_GPIO_PIN_PD12                     108
#define SUNXI_GPIO_PIN_PD13                     109
#define SUNXI_GPIO_PIN_PD14                     110
#define SUNXI_GPIO_PIN_PD15                     111
#define SUNXI_GPIO_PIN_PD16                     112
#define SUNXI_GPIO_PIN_PD17                     113
#define SUNXI_GPIO_PIN_PD18                     114
#define SUNXI_GPIO_PIN_PD19                     115
#define SUNXI_GPIO_PIN_PD20                     116
#define SUNXI_GPIO_PIN_PD21                     117
#define SUNXI_GPIO_PIN_PD22                     118
#define SUNXI_GPIO_PIN_PD23                     119
#define SUNXI_GPIO_PIN_PD24                     120
#define SUNXI_GPIO_PIN_PD25                     121
#define SUNXI_GPIO_PIN_PD26                     122
#define SUNXI_GPIO_PIN_PD27                     123
#define SUNXI_GPIO_PIN_PD28                     124
#define SUNXI_GPIO_PIN_PD29                     125
#define SUNXI_GPIO_PIN_PD30                     126
#define SUNXI_GPIO_PIN_PD31                     127
#define SUNXI_GPIO_PIN_PE0                      128
#define SUNXI_GPIO_PIN_PE1                      129
#define SUNXI_GPIO_PIN_PE2                      130
#define SUNXI_GPIO_PIN_PE3                      131
#define SUNXI_GPIO_PIN_PE4                      132
#define SUNXI_GPIO_PIN_PE5                      133
#define SUNXI_GPIO_PIN_PE6                      134
#define SUNXI_GPIO_PIN_PE7                      135
#define SUNXI_GPIO_PIN_PE8                      136
#define SUNXI_GPIO_PIN_PE9                      137
#define SUNXI_GPIO_PIN_PE10                     138
#define SUNXI_GPIO_PIN_PE11                     139
#define SUNXI_GPIO_PIN_PE12                     140
#define SUNXI_GPIO_PIN_PE13                     141
#define SUNXI_GPIO_PIN_PE14                     142
#define SUNXI_GPIO_PIN_PE15                     143
#define SUNXI_GPIO_PIN_PE16                     144
#define SUNXI_GPIO_PIN_PE17                     145
#define SUNXI_GPIO_PIN_PE18                     146
#define SUNXI_GPIO_PIN_PE19                     147
#define SUNXI_GPIO_PIN_PE20                     148
#define SUNXI_GPIO_PIN_PE21                     149
#define SUNXI_GPIO_PIN_PE22                     150
#define SUNXI_GPIO_PIN_PE23                     151
#define SUNXI_GPIO_PIN_PE24                     152
#define SUNXI_GPIO_PIN_PE25                     153
#define SUNXI_GPIO_PIN_PE26                     154
#define SUNXI_GPIO_PIN_PE27                     155
#define SUNXI_GPIO_PIN_PE28                     156
#define SUNXI_GPIO_PIN_PE29                     157
#define SUNXI_GPIO_PIN_PE30                     158
#define SUNXI_GPIO_PIN_PE31                     159
#define SUNXI_GPIO_PIN_PF0                      160
#define SUNXI_GPIO_PIN_PF1                      161
#define SUNXI_GPIO_PIN_PF2                      162
#define SUNXI_GPIO_PIN_PF3                      163
#define SUNXI_GPIO_PIN_PF4                      164
#define SUNXI_GPIO_PIN_PF5                      165
#define SUNXI_GPIO_PIN_PF6                      166
#define SUNXI_GPIO_PIN_PF7                      167
#define SUNXI_GPIO_PIN_PF8                      168
#define SUNXI_GPIO_PIN_PF9                      169
#define SUNXI_GPIO_PIN_PF10                     170
#define SUNXI_GPIO_PIN_PF11                     171
#define SUNXI_GPIO_PIN_PF12                     172
#define SUNXI_GPIO_PIN_PF13                     173
#define SUNXI_GPIO_PIN_PF14                     174
#define SUNXI_GPIO_PIN_PF15                     175
#define SUNXI_GPIO_PIN_PF16                     176
#define SUNXI_GPIO_PIN_PF17                     177
#define SUNXI_GPIO_PIN_PF18                     178
#define SUNXI_GPIO_PIN_PF19                     179
#define SUNXI_GPIO_PIN_PF20                     180
#define SUNXI_GPIO_PIN_PF21                     181
#define SUNXI_GPIO_PIN_PF22                     182
#define SUNXI_GPIO_PIN_PF23                     183
#define SUNXI_GPIO_PIN_PF24                     184
#define SUNXI_GPIO_PIN_PF25                     185
#define SUNXI_GPIO_PIN_PF26                     186
#define SUNXI_GPIO_PIN_PF27                     187
#define SUNXI_GPIO_PIN_PF28                     188
#define SUNXI_GPIO_PIN_PF29                     189
#define SUNXI_GPIO_PIN_PF30                     190
#define SUNXI_GPIO_PIN_PF31                     191
#define SUNXI_GPIO_PIN_PG0                      192
#define SUNXI_GPIO_PIN_PG1                      193
#define SUNXI_GPIO_PIN_PG2                      194
#define SUNXI_GPIO_PIN_PG3                      195
#define SUNXI_GPIO_PIN_PG4                      196
#define SUNXI_GPIO_PIN_PG5                      197
#define SUNXI_GPIO_PIN_PG6                      198
#define SUNXI_GPIO_PIN_PG7                      199
#define SUNXI_GPIO_PIN_PG8                      200
#define SUNXI_GPIO_PIN_PG9                      201
#define SUNXI_GPIO_PIN_PG10                     202
#define SUNXI_GPIO_PIN_PG11                     203
#define SUNXI_GPIO_PIN_PG12                     204
#define SUNXI_GPIO_PIN_PG13                     205
#define SUNXI_GPIO_PIN_PG14                     206
#define SUNXI_GPIO_PIN_PG15                     207
#define SUNXI_GPIO_PIN_PG16                     208
#define SUNXI_GPIO_PIN_PG17                     209
#define SUNXI_GPIO_PIN_PG18                     210
#define SUNXI_GPIO_PIN_PG19                     211
#define SUNXI_GPIO_PIN_PG20                     212
#define SUNXI_GPIO_PIN_PG21                     213
#define SUNXI_GPIO_PIN_PG22                     214
#define SUNXI_GPIO_PIN_PG23                     215
#define SUNXI_GPIO_PIN_PG24                     216
#define SUNXI_GPIO_PIN_PG25                     217
#define SUNXI_GPIO_PIN_PG26                     218
#define SUNXI_GPIO_PIN_PG27                     219
#define SUNXI_GPIO_PIN_PG28                     220
#define SUNXI_GPIO_PIN_PG29                     221
#define SUNXI_GPIO_PIN_PG30                     222
#define SUNXI_GPIO_PIN_PG31                     223
#define SUNXI_GPIO_PIN_PH0                      224
#define SUNXI_GPIO_PIN_PH1                      225
#define SUNXI_GPIO_PIN_PH2                      226
#define SUNXI_GPIO_PIN_PH3                      227
#define SUNXI_GPIO_PIN_PH4                      228
#define SUNXI_GPIO_PIN_PH5                      229
#define SUNXI_GPIO_PIN_PH6                      230
#define SUNXI_GPIO_PIN_PH7                      231
#define SUNXI_GPIO_PIN_PH8                      232
#define SUNXI_GPIO_PIN_PH9                      233
#define SUNXI_GPIO_PIN_PH10                     234
#define SUNXI_GPIO_PIN_PH11                     235
#define SUNXI_GPIO_PIN_PH12                     236
#define SUNXI_GPIO_PIN_PH13                     237
#define SUNXI_GPIO_PIN_PH14                     238
#define SUNXI_GPIO_PIN_PH15                     239
#define SUNXI_GPIO_PIN_PH16                     240
#define SUNXI_GPIO_PIN_PH17                     241
#define SUNXI_GPIO_PIN_PH18                     242
#define SUNXI_GPIO_PIN_PH19                     243
#define SUNXI_GPIO_PIN_PH20                     244
#define SUNXI_GPIO_PIN_PH21                     245
#define SUNXI_GPIO_PIN_PH22                     246
#define SUNXI_GPIO_PIN_PH23                     247
#define SUNXI_GPIO_PIN_PH24                     248
#define SUNXI_GPIO_PIN_PH25                     249
#define SUNXI_GPIO_PIN_PH26                     250
#define SUNXI_GPIO_PIN_PH27                     251
#define SUNXI_GPIO_PIN_PH28                     252
#define SUNXI_GPIO_PIN_PH29                     253
#define SUNXI_GPIO_PIN_PH30                     254
#define SUNXI_GPIO_PIN_PH31                     255


/****************************************************************************************/
/* Prototypes                                                                           */
/****************************************************************************************/

int sunxi_gpio_init();
int sunxi_gpio_set_cfgpin(unsigned int pin, unsigned int val);
int sunxi_gpio_get_cfgpin(unsigned int pin);
int sunxi_gpio_input(unsigned int pin);
int sunxi_gpio_output(unsigned int pin, unsigned int val);


#endif
