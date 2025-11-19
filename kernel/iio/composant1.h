#define COMPOSANT_CHANNELS			2

/**
 * struct composant_chip_info - chip specific information
 * @channels:		channel spec for the DAC
 * @int_vref_mv:	internal reference voltage
 */

struct composant_chip_info {
	const struct iio_chan_spec	*channels;
};

/**
 * struct composant_state - driver instance specific data
 * @indio_dev:		the industrial I/O device
 * @us:			spi_device
 * @chip_info:		chip model specific constants, available modes etc
 * @reg:		supply regulator
 * @vref_mv:		actual reference voltage used
 * @pwr_down_mask	power down mask
 * @pwr_down_mode	current power down mode
 */

struct composant_state {
//	struct spi_device		*us;
	struct platform_device		*us;
	const struct composant_chip_info *chip_info;
	struct regulator		*reg;
	unsigned short			vref_mv;
	unsigned			pwr_down_mask;
	unsigned			pwr_down_mode;
};

/**
 * composant_supported_device_ids:
 * The COMPOSANT parts are available in different options.
 */

enum composant_supported_device_ids {
	ID_COMPOSANT12,
};
