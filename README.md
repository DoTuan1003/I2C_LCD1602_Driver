# I2C_LCD1602 device Driver linux for beagle ball black
how to run ?
1. add device tree to am335x-bone-common.dtsi and build(./tools/rebuild) to .dtb
&i2c1 {
	pinctrl-names = "default";
	pinctrl-0 = <&i2c1_pins>;

	status = "okay";
	clock-frequency = <100000>;
	symlink = "bone/i2c/1";

	lcd1602: lcd1602@27 {
		compatible = "b3,lcd1602";
		#address-cells = <1>;
		#size-cells = <0>;
		reg = <0x27>; //dia chi cua I2c
		status = "okay";
		};
	};
    	i2c1_pins: pinmux_i2c1_pins {
		pinctrl-single,pins = <
			AM33XX_PADCONF(AM335X_PIN_SPI0_CS0, PIN_INPUT_PULLUP, MUX_MODE2)	// su dung chan P9_17 lam SCl
			AM33XX_PADCONF(AM335X_PIN_SPI0_D1, PIN_INPUT_PULLUP, MUX_MODE2)	 // su dung chan P9_18 lam SDA
		>;
	};
2.cd I2c_lcd_driver/ run command  "make" to recive LcdI2c.ko
3.cd App/ run command  "make" to recive App_test
4.insmod LcdI2c.ko and run ./App