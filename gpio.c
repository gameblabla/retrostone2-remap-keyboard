/****************************************************************************************/
/* SUNXI GPIO library interface                                                         */
/****************************************************************************************/

/****************************************************************************************/
/* Includes                                                                             */
/****************************************************************************************/

#include "gpio.h"


/****************************************************************************************/
/* Definitions                                                                          */
/****************************************************************************************/

/* SUNXI GPIO control IO base address */
#define SUNXI_GPIO_IO_BASE                      0x01c20800

/* Macros used to configure GPIOs */
#define SUNXI_GPIO_BANK(pin)                    ((pin) >> 5)
#define SUNXI_GPIO_NUM(pin)                     ((pin) & 0x1F)
#define SUNXI_GPIO_CFG_INDEX(pin)               (((pin) & 0x1F) >> 3)
#define SUNXI_GPIO_CFG_OFFSET(pin)              ((((pin) & 0x1F) & 0x7) << 2)

/* SUNXI GPIO Bank */
struct sunxi_gpio_bank {
  volatile unsigned int cfg[4];
  volatile unsigned int dat;
  volatile unsigned int drv[2];
  volatile unsigned int pull[2];
};

/* SUNXI GPIO Interrupt control */
struct sunxi_gpio_int {
  volatile unsigned int cfg[3];
  volatile unsigned int ctl;
  volatile unsigned int sta;
  volatile unsigned int deb;
};

/* SUNXI GPIO Registers */
struct sunxi_gpio_reg {
  volatile struct sunxi_gpio_bank gpio_bank[9];
  volatile unsigned char res[0xbc];
  volatile struct sunxi_gpio_int gpio_int;
};


/****************************************************************************************/
/* Global variables                                                                     */
/****************************************************************************************/

/* SUNXI GPIO registers */
static volatile struct sunxi_gpio_reg *sunxi_gpio_registers = NULL;


/****************************************************************************************/
/* Exported functions                                                                   */
/****************************************************************************************/

/**
 * Initialize GPIO interface, to be called once
 * @return 0 if the function succeeds, error code otherwise
 */
int sunxi_gpio_init() {
  
  int fd;
  unsigned int page_size, page_mask;
  unsigned int addr_start, addr_offset;
  void *pc;

  /* Open device */
  fd = open("/dev/mem", O_RDWR);
  if (fd < 0) {
    return -errno;
  }
  
  /* Map to device */
  page_size = sysconf(_SC_PAGESIZE);
  page_mask = (~(page_size-1));
  addr_start = SUNXI_GPIO_IO_BASE & page_mask;
  addr_offset = SUNXI_GPIO_IO_BASE & ~page_mask;
  pc = (void *)mmap(NULL, (((sizeof(struct sunxi_gpio_reg) + addr_offset) / page_size) + 1) * page_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, addr_start);
  if (pc == MAP_FAILED) {
    return -errno;
  }

  /* Retrieve registers address used later in this library */
  sunxi_gpio_registers = (struct sunxi_gpio_reg *)(pc + addr_offset);

  /* Close device */
  close(fd);
  
  return 0;
}

/**
 * Set pin configuration
 * @param pin Expected pin, see SUNXI_GPIO_PIN macros
 * @param val Expected function, SUNXI_GPIO_INPUT or SUNXI_GPIO_OUTPUT
 * @return 0 if the function succeeds, error code otherwise
 */
int sunxi_gpio_set_cfgpin(unsigned int pin, unsigned int val) {
  
  unsigned int cfg;
  unsigned int bank = SUNXI_GPIO_BANK(pin);
  unsigned int index = SUNXI_GPIO_CFG_INDEX(pin);
  unsigned int offset = SUNXI_GPIO_CFG_OFFSET(pin);

  /* Check if initialization has been performed */
  if (sunxi_gpio_registers == NULL) {
    return -EPERM;
  }

  /* Set pin configuration */
  volatile struct sunxi_gpio_bank *pio = &(sunxi_gpio_registers->gpio_bank[bank]);
  cfg = *(&pio->cfg[0] + index);
  cfg &= ~(0xf << offset);
  cfg |= val << offset;
  *(&pio->cfg[0] + index) = cfg;

  return 0;
}

/**
 * Get pin configuration
 * @param pin Expected pin, see SUNXI_GPIO_PIN macros
 * @return Pin function if the function succeeds, error code otherwise
 */
int sunxi_gpio_get_cfgpin(unsigned int pin) {

  unsigned int cfg;
  unsigned int bank = SUNXI_GPIO_BANK(pin);
  unsigned int index = SUNXI_GPIO_CFG_INDEX(pin);
  unsigned int offset = SUNXI_GPIO_CFG_OFFSET(pin);
  
  /* Check if initialization has been performed */
  if (sunxi_gpio_registers == NULL) {
    return -EPERM;
  }
  
  /* Get pin configuration */
  volatile struct sunxi_gpio_bank *pio = &(sunxi_gpio_registers->gpio_bank[bank]);
  cfg = *(&pio->cfg[0] + index);
  cfg >>= offset;
  return (cfg & 0xf);
}

/**
 * Get pin input value
 * @param pin Expected pin, see SUNXI_GPIO_PIN macros
 * @return Pin input value if the function succeeds, error code otherwise
 */
int sunxi_gpio_input(unsigned int pin) {
  
  unsigned int dat;
  unsigned int bank = SUNXI_GPIO_BANK(pin);
  unsigned int num = SUNXI_GPIO_NUM(pin);
  
  /* Check if initialization has been performed */
  if (sunxi_gpio_registers == NULL) {
    return -EPERM;
  }
  
  /* Get pin value */
  volatile struct sunxi_gpio_bank *pio = &(sunxi_gpio_registers->gpio_bank[bank]);
  dat = *(&pio->dat);
  dat >>= num;
  return (dat & 0x1);
}

/**
 * Set pin output value
 * @param pin Expected pin, see SUNXI_GPIO_PIN macros
 * @param val Expected pin value, 0 or 1
 * @return 0 if the function succeeds, error code otherwise
 */
int sunxi_gpio_output(unsigned int pin, unsigned int val) {

  unsigned int bank = SUNXI_GPIO_BANK(pin);
  unsigned int num = SUNXI_GPIO_NUM(pin);
  
  /* Check if initialization has been performed */
  if (sunxi_gpio_registers == NULL) {
    return -EPERM;
  }
  
  /* Set pin value */
  volatile struct sunxi_gpio_bank *pio = &(sunxi_gpio_registers->gpio_bank[bank]);
  if (val)
    *(&pio->dat) |= 1 << num;
  else
    *(&pio->dat) &= ~(1 << num);
  return 0;
}
